/**
 *
 *  @file SDLOpenGLRenderer.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/game
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Vix.cpp
 *
 */

#include <vix/game/backends/sdl/SDLOpenGLRenderer.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <string>

#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL2/SDL_image.h>

#include <vix/game/AssetType.hpp>
#include <vix/game/GameError.hpp>

namespace
{
  PFNGLGENVERTEXARRAYSPROC gl_gen_vertex_arrays = nullptr;
  PFNGLBINDVERTEXARRAYPROC gl_bind_vertex_array = nullptr;
  PFNGLDELETEVERTEXARRAYSPROC gl_delete_vertex_arrays = nullptr;

  PFNGLGENBUFFERSPROC gl_gen_buffers = nullptr;
  PFNGLBINDBUFFERPROC gl_bind_buffer = nullptr;
  PFNGLBUFFERDATAPROC gl_buffer_data = nullptr;
  PFNGLBUFFERSUBDATAPROC gl_buffer_sub_data = nullptr;
  PFNGLDELETEBUFFERSPROC gl_delete_buffers = nullptr;

  PFNGLENABLEVERTEXATTRIBARRAYPROC gl_enable_vertex_attrib_array = nullptr;
  PFNGLVERTEXATTRIBPOINTERPROC gl_vertex_attrib_pointer = nullptr;

  PFNGLCREATESHADERPROC gl_create_shader = nullptr;
  PFNGLSHADERSOURCEPROC gl_shader_source = nullptr;
  PFNGLCOMPILESHADERPROC gl_compile_shader = nullptr;
  PFNGLGETSHADERIVPROC gl_get_shader_iv = nullptr;
  PFNGLGETSHADERINFOLOGPROC gl_get_shader_info_log = nullptr;
  PFNGLDELETESHADERPROC gl_delete_shader = nullptr;

  PFNGLCREATEPROGRAMPROC gl_create_program = nullptr;
  PFNGLATTACHSHADERPROC gl_attach_shader = nullptr;
  PFNGLLINKPROGRAMPROC gl_link_program = nullptr;
  PFNGLGETPROGRAMIVPROC gl_get_program_iv = nullptr;
  PFNGLGETPROGRAMINFOLOGPROC gl_get_program_info_log = nullptr;
  PFNGLDELETEPROGRAMPROC gl_delete_program = nullptr;
  PFNGLUSEPROGRAMPROC gl_use_program = nullptr;

  PFNGLGETUNIFORMLOCATIONPROC gl_get_uniform_location = nullptr;
  PFNGLUNIFORMMATRIX4FVPROC gl_uniform_matrix_4fv = nullptr;
  PFNGLUNIFORM1IPROC gl_uniform_1i = nullptr;

  void *load_gl_proc(const char *name)
  {
    return SDL_GL_GetProcAddress(name);
  }

  template <typename T>
  bool load_gl_function(T &target, const char *name)
  {
    target = reinterpret_cast<T>(load_gl_proc(name));
    return target != nullptr;
  }

  std::string shader_error(GLuint shader)
  {
    GLint length = 0;
    gl_get_shader_iv(shader, GL_INFO_LOG_LENGTH, &length);

    if (length <= 0)
    {
      return "unknown shader compile error";
    }

    std::string out(static_cast<std::size_t>(length), '\0');
    gl_get_shader_info_log(shader, length, nullptr, out.data());
    return out;
  }

  std::string program_error(GLuint program)
  {
    GLint length = 0;
    gl_get_program_iv(program, GL_INFO_LOG_LENGTH, &length);

    if (length <= 0)
    {
      return "unknown shader link error";
    }

    std::string out(static_cast<std::size_t>(length), '\0');
    gl_get_program_info_log(program, length, nullptr, out.data());
    return out;
  }

  bool compile_shader(
      GLenum type,
      const char *source,
      GLuint &out)
  {
    out = gl_create_shader(type);
    if (out == 0)
    {
      return false;
    }

    gl_shader_source(out, 1, &source, nullptr);
    gl_compile_shader(out);

    GLint ok = GL_FALSE;
    gl_get_shader_iv(out, GL_COMPILE_STATUS, &ok);

    return ok == GL_TRUE;
  }
}

namespace vix::game::sdl
{
  SDLOpenGLRenderer::~SDLOpenGLRenderer()
  {
    shutdown();
  }

  GameBoolResult SDLOpenGLRenderer::init(Window &window)
  {
    if (!window.open())
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "cannot initialize SDL OpenGL renderer with a closed window");
    }

    auto *native_window = static_cast<SDL_Window *>(window.native_handle());
    if (native_window == nullptr)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "SDL OpenGL renderer requires an SDL window native handle");
    }

    if (context_ != nullptr)
    {
      shutdown();
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    context_ = SDL_GL_CreateContext(native_window);
    if (context_ == nullptr)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          SDL_GetError());
    }

    if (SDL_GL_MakeCurrent(native_window, context_) != 0)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          SDL_GetError());
    }

    SDL_GL_SetSwapInterval(1);

    auto loaded = load_gl_functions();
    if (!loaded)
    {
      return loaded.error();
    }

    window_ = native_window;
    width_ = window.width();
    height_ = window.height();

    if (width_ == 0 || height_ == 0)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "SDL OpenGL renderer size must be greater than zero");
    }

    auto pipeline = create_pipeline();
    if (!pipeline)
    {
      return pipeline.error();
    }

    glViewport(
        0,
        0,
        static_cast<GLsizei>(width_),
        static_cast<GLsizei>(height_));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initialized_ = true;
    frame_active_ = false;
    sprite_count_ = 0;
    uploaded_texture_count_ = 0;

    return true;
  }

  GameBoolResult SDLOpenGLRenderer::upload_texture(const Asset &asset)
  {
    if (!initialized_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "SDL OpenGL renderer is not initialized");
    }

    if (!asset.valid())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "texture asset id is invalid");
    }

    if (asset.type() != AssetType::Image)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "texture asset must be an image");
    }

    if (!asset.binary() || asset.bytes().empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "texture asset must contain binary image data");
    }

    if (textures_.find(asset.id()) != textures_.end())
    {
      return true;
    }

    SDL_RWops *rw = SDL_RWFromConstMem(
        asset.bytes().data(),
        static_cast<int>(asset.bytes().size()));

    if (rw == nullptr)
    {
      return make_game_error(
          GameErrorCode::AssetLoadFailed,
          SDL_GetError());
    }

    SDL_Surface *loaded_surface = IMG_Load_RW(rw, 1);
    if (loaded_surface == nullptr)
    {
      return make_game_error(
          GameErrorCode::AssetLoadFailed,
          IMG_GetError());
    }

    SDL_Surface *surface = SDL_ConvertSurfaceFormat(
        loaded_surface,
        SDL_PIXELFORMAT_ABGR8888,
        0);

    SDL_FreeSurface(loaded_surface);

    if (surface == nullptr)
    {
      return make_game_error(
          GameErrorCode::AssetLoadFailed,
          SDL_GetError());
    }

    GLuint texture = 0;
    glGenTextures(1, &texture);

    if (texture == 0)
    {
      SDL_FreeSurface(surface);
      return make_game_error(
          GameErrorCode::AssetLoadFailed,
          "failed to create OpenGL texture");
    }

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        surface->w,
        surface->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        surface->pixels);

    TextureRecord record;
    record.id = texture;
    record.width = static_cast<std::uint32_t>(surface->w);
    record.height = static_cast<std::uint32_t>(surface->h);

    SDL_FreeSurface(surface);

    textures_.emplace(asset.id(), record);
    uploaded_texture_count_ = textures_.size();

    return true;
  }

  void SDLOpenGLRenderer::shutdown() noexcept
  {
    destroy_textures();
    destroy_pipeline();

    if (context_ != nullptr)
    {
      SDL_GL_DeleteContext(context_);
      context_ = nullptr;
    }

    window_ = nullptr;
    initialized_ = false;
    frame_active_ = false;
    width_ = 0;
    height_ = 0;
    sprite_count_ = 0;
    uploaded_texture_count_ = 0;
  }

  void SDLOpenGLRenderer::begin_frame()
  {
    if (!initialized_)
    {
      return;
    }

    frame_active_ = true;
    sprite_count_ = 0;
  }

  void SDLOpenGLRenderer::end_frame()
  {
    if (!initialized_ || window_ == nullptr)
    {
      return;
    }

    SDL_GL_SwapWindow(window_);
    frame_active_ = false;
  }

  void SDLOpenGLRenderer::clear(Color color)
  {
    if (!initialized_)
    {
      return;
    }

    const auto clamped = color.clamped();

    glClearColor(
        std::clamp(clamped.r, 0.0F, 1.0F),
        std::clamp(clamped.g, 0.0F, 1.0F),
        std::clamp(clamped.b, 0.0F, 1.0F),
        std::clamp(clamped.a, 0.0F, 1.0F));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void SDLOpenGLRenderer::draw_sprite(const DrawSpriteCommand &command)
  {
    if (!initialized_ || !frame_active_ || !command.valid())
    {
      return;
    }

    const AssetId id = command.sprite.texture.asset_id();

    const auto texture_it = textures_.find(id);
    if (texture_it == textures_.end() || texture_it->second.id == 0)
    {
      return;
    }

    const auto &texture = texture_it->second;
    const auto &transform = command.transform;
    const auto &sprite = command.sprite;

    float source_x = sprite.source.x;
    float source_y = sprite.source.y;
    float source_w = sprite.source.width;
    float source_h = sprite.source.height;

    if (source_w <= 0.0F || source_h <= 0.0F)
    {
      source_x = 0.0F;
      source_y = 0.0F;
      source_w = static_cast<float>(texture.width);
      source_h = static_cast<float>(texture.height);
    }

    float u0 = source_x / static_cast<float>(texture.width);
    float v0 = source_y / static_cast<float>(texture.height);
    float u1 = (source_x + source_w) / static_cast<float>(texture.width);
    float v1 = (source_y + source_h) / static_cast<float>(texture.height);

    if (sprite.flip_x)
    {
      std::swap(u0, u1);
    }

    if (sprite.flip_y)
    {
      std::swap(v0, v1);
    }

    const float width =
        sprite.size.x > 0.0F ? sprite.size.x : static_cast<float>(texture.width);

    const float height =
        sprite.size.y > 0.0F ? sprite.size.y : static_cast<float>(texture.height);

    const float scaled_width = width * transform.scale.x;
    const float scaled_height = height * transform.scale.y;

    const float left = -transform.origin.x;
    const float top = -transform.origin.y;
    const float right = left + scaled_width;
    const float bottom = top + scaled_height;

    const float cos_r = std::cos(transform.rotation);
    const float sin_r = std::sin(transform.rotation);

    auto rotate_x = [&](float x, float y)
    {
      return transform.position.x + (x * cos_r - y * sin_r);
    };

    auto rotate_y = [&](float x, float y)
    {
      return transform.position.y + (x * sin_r + y * cos_r);
    };

    std::array<Vertex, 4> vertices{
        Vertex{
            rotate_x(left, top),
            rotate_y(left, top),
            u0,
            v0,
            sprite.tint.r,
            sprite.tint.g,
            sprite.tint.b,
            sprite.tint.a,
        },
        Vertex{
            rotate_x(right, top),
            rotate_y(right, top),
            u1,
            v0,
            sprite.tint.r,
            sprite.tint.g,
            sprite.tint.b,
            sprite.tint.a,
        },
        Vertex{
            rotate_x(right, bottom),
            rotate_y(right, bottom),
            u1,
            v1,
            sprite.tint.r,
            sprite.tint.g,
            sprite.tint.b,
            sprite.tint.a,
        },
        Vertex{
            rotate_x(left, bottom),
            rotate_y(left, bottom),
            u0,
            v1,
            sprite.tint.r,
            sprite.tint.g,
            sprite.tint.b,
            sprite.tint.a,
        },
    };

    use_pipeline();

    const auto projection = projection_matrix();
    gl_uniform_matrix_4fv(
        projection_location_,
        1,
        GL_FALSE,
        projection.data());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    gl_uniform_1i(texture_location_, 0);

    gl_bind_vertex_array(vao_);
    gl_bind_buffer(GL_ARRAY_BUFFER, vbo_);
    gl_buffer_sub_data(
        GL_ARRAY_BUFFER,
        0,
        static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
        vertices.data());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    gl_bind_vertex_array(0);

    ++sprite_count_;
  }

  void SDLOpenGLRenderer::resize(
      std::uint32_t width,
      std::uint32_t height)
  {
    if (width == 0 || height == 0)
    {
      return;
    }

    width_ = width;
    height_ = height;

    if (initialized_)
    {
      glViewport(
          0,
          0,
          static_cast<GLsizei>(width_),
          static_cast<GLsizei>(height_));
    }
  }

  bool SDLOpenGLRenderer::initialized() const noexcept
  {
    return initialized_;
  }

  std::uint32_t SDLOpenGLRenderer::width() const noexcept
  {
    return width_;
  }

  std::uint32_t SDLOpenGLRenderer::height() const noexcept
  {
    return height_;
  }

  const char *SDLOpenGLRenderer::backend_name() const noexcept
  {
    return "sdl-opengl";
  }

  void *SDLOpenGLRenderer::native_handle() noexcept
  {
    return context_;
  }

  const void *SDLOpenGLRenderer::native_handle() const noexcept
  {
    return context_;
  }

  bool SDLOpenGLRenderer::frame_active() const noexcept
  {
    return frame_active_;
  }

  std::uint64_t SDLOpenGLRenderer::sprite_count() const noexcept
  {
    return sprite_count_;
  }

  std::uint64_t SDLOpenGLRenderer::uploaded_texture_count() const noexcept
  {
    return uploaded_texture_count_;
  }

  GameBoolResult SDLOpenGLRenderer::load_gl_functions()
  {
    const bool ok =
        load_gl_function(gl_gen_vertex_arrays, "glGenVertexArrays") &&
        load_gl_function(gl_bind_vertex_array, "glBindVertexArray") &&
        load_gl_function(gl_delete_vertex_arrays, "glDeleteVertexArrays") &&
        load_gl_function(gl_gen_buffers, "glGenBuffers") &&
        load_gl_function(gl_bind_buffer, "glBindBuffer") &&
        load_gl_function(gl_buffer_data, "glBufferData") &&
        load_gl_function(gl_buffer_sub_data, "glBufferSubData") &&
        load_gl_function(gl_delete_buffers, "glDeleteBuffers") &&
        load_gl_function(gl_enable_vertex_attrib_array, "glEnableVertexAttribArray") &&
        load_gl_function(gl_vertex_attrib_pointer, "glVertexAttribPointer") &&
        load_gl_function(gl_create_shader, "glCreateShader") &&
        load_gl_function(gl_shader_source, "glShaderSource") &&
        load_gl_function(gl_compile_shader, "glCompileShader") &&
        load_gl_function(gl_get_shader_iv, "glGetShaderiv") &&
        load_gl_function(gl_get_shader_info_log, "glGetShaderInfoLog") &&
        load_gl_function(gl_delete_shader, "glDeleteShader") &&
        load_gl_function(gl_create_program, "glCreateProgram") &&
        load_gl_function(gl_attach_shader, "glAttachShader") &&
        load_gl_function(gl_link_program, "glLinkProgram") &&
        load_gl_function(gl_get_program_iv, "glGetProgramiv") &&
        load_gl_function(gl_get_program_info_log, "glGetProgramInfoLog") &&
        load_gl_function(gl_delete_program, "glDeleteProgram") &&
        load_gl_function(gl_use_program, "glUseProgram") &&
        load_gl_function(gl_get_uniform_location, "glGetUniformLocation") &&
        load_gl_function(gl_uniform_matrix_4fv, "glUniformMatrix4fv") &&
        load_gl_function(gl_uniform_1i, "glUniform1i");

    if (!ok)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "failed to load required OpenGL functions");
    }

    return true;
  }

  GameBoolResult SDLOpenGLRenderer::create_pipeline()
  {
    auto shaders = create_shader_program();
    if (!shaders)
    {
      return shaders.error();
    }

    auto buffers = create_buffers();
    if (!buffers)
    {
      return buffers.error();
    }

    return true;
  }

  GameBoolResult SDLOpenGLRenderer::create_shader_program()
  {
    static constexpr const char *vertex_shader_source = R"(
#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec4 a_color;

uniform mat4 u_projection;

out vec2 v_uv;
out vec4 v_color;

void main()
{
  gl_Position = u_projection * vec4(a_position, 0.0, 1.0);
  v_uv = a_uv;
  v_color = a_color;
}
)";

    static constexpr const char *fragment_shader_source = R"(
#version 330 core

in vec2 v_uv;
in vec4 v_color;

uniform sampler2D u_texture;

out vec4 frag_color;

void main()
{
  frag_color = texture(u_texture, v_uv) * v_color;
}
)";

    GLuint vertex_shader = 0;
    if (!compile_shader(GL_VERTEX_SHADER, vertex_shader_source, vertex_shader))
    {
      const auto error = shader_error(vertex_shader);
      if (vertex_shader != 0)
      {
        gl_delete_shader(vertex_shader);
      }

      return make_game_error(
          GameErrorCode::InvalidState,
          error);
    }

    GLuint fragment_shader = 0;
    if (!compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source, fragment_shader))
    {
      const auto error = shader_error(fragment_shader);

      gl_delete_shader(vertex_shader);

      if (fragment_shader != 0)
      {
        gl_delete_shader(fragment_shader);
      }

      return make_game_error(
          GameErrorCode::InvalidState,
          error);
    }

    shader_program_ = gl_create_program();
    gl_attach_shader(shader_program_, vertex_shader);
    gl_attach_shader(shader_program_, fragment_shader);
    gl_link_program(shader_program_);

    gl_delete_shader(vertex_shader);
    gl_delete_shader(fragment_shader);

    GLint ok = GL_FALSE;
    gl_get_program_iv(shader_program_, GL_LINK_STATUS, &ok);

    if (ok != GL_TRUE)
    {
      const auto error = program_error(shader_program_);
      gl_delete_program(shader_program_);
      shader_program_ = 0;

      return make_game_error(
          GameErrorCode::InvalidState,
          error);
    }

    projection_location_ =
        gl_get_uniform_location(shader_program_, "u_projection");

    texture_location_ =
        gl_get_uniform_location(shader_program_, "u_texture");

    if (projection_location_ < 0 || texture_location_ < 0)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "failed to locate OpenGL shader uniforms");
    }

    return true;
  }

  GameBoolResult SDLOpenGLRenderer::create_buffers()
  {
    static constexpr std::array<std::uint32_t, 6> indices{
        0U,
        1U,
        2U,
        2U,
        3U,
        0U,
    };

    gl_gen_vertex_arrays(1, &vao_);
    gl_gen_buffers(1, &vbo_);
    gl_gen_buffers(1, &ebo_);

    if (vao_ == 0 || vbo_ == 0 || ebo_ == 0)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "failed to create OpenGL buffers");
    }

    gl_bind_vertex_array(vao_);

    gl_bind_buffer(GL_ARRAY_BUFFER, vbo_);
    gl_buffer_data(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(sizeof(Vertex) * 4),
        nullptr,
        GL_DYNAMIC_DRAW);

    gl_bind_buffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    gl_buffer_data(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(std::uint32_t)),
        indices.data(),
        GL_STATIC_DRAW);

    gl_enable_vertex_attrib_array(0);
    gl_vertex_attrib_pointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(sizeof(Vertex)),
        reinterpret_cast<const void *>(0));

    gl_enable_vertex_attrib_array(1);
    gl_vertex_attrib_pointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(sizeof(Vertex)),
        reinterpret_cast<const void *>(sizeof(float) * 2));

    gl_enable_vertex_attrib_array(2);
    gl_vertex_attrib_pointer(
        2,
        4,
        GL_FLOAT,
        GL_FALSE,
        static_cast<GLsizei>(sizeof(Vertex)),
        reinterpret_cast<const void *>(sizeof(float) * 4));

    gl_bind_vertex_array(0);

    return true;
  }

  std::array<float, 16> SDLOpenGLRenderer::projection_matrix() const noexcept
  {
    const float left = 0.0F;
    const float right = static_cast<float>(width_);
    const float bottom = static_cast<float>(height_);
    const float top = 0.0F;
    const float near_z = -1.0F;
    const float far_z = 1.0F;

    return {
        2.0F / (right - left),
        0.0F,
        0.0F,
        0.0F,

        0.0F,
        2.0F / (top - bottom),
        0.0F,
        0.0F,

        0.0F,
        0.0F,
        -2.0F / (far_z - near_z),
        0.0F,

        -(right + left) / (right - left),
        -(top + bottom) / (top - bottom),
        -(far_z + near_z) / (far_z - near_z),
        1.0F,
    };
  }

  void SDLOpenGLRenderer::destroy_pipeline() noexcept
  {
    if (ebo_ != 0)
    {
      gl_delete_buffers(1, &ebo_);
      ebo_ = 0;
    }

    if (vbo_ != 0)
    {
      gl_delete_buffers(1, &vbo_);
      vbo_ = 0;
    }

    if (vao_ != 0)
    {
      gl_delete_vertex_arrays(1, &vao_);
      vao_ = 0;
    }

    if (shader_program_ != 0)
    {
      gl_delete_program(shader_program_);
      shader_program_ = 0;
    }

    projection_location_ = -1;
    texture_location_ = -1;
  }

  void SDLOpenGLRenderer::destroy_textures() noexcept
  {
    for (auto &[asset_id, texture] : textures_)
    {
      (void)asset_id;

      if (texture.id != 0)
      {
        const GLuint id = texture.id;
        glDeleteTextures(1, &id);
      }
    }

    textures_.clear();
    uploaded_texture_count_ = 0;
  }

  void SDLOpenGLRenderer::use_pipeline()
  {
    gl_use_program(shader_program_);
  }

} // namespace vix::game::sdl
