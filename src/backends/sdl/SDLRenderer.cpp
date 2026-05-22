/**
 *
 *  @file SDLRenderer.cpp
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

#include <vix/game/backends/sdl/SDLRenderer.hpp>

#include <algorithm>

#include <vix/game/AssetType.hpp>
#include <vix/game/GameError.hpp>
#include <SDL2/SDL_image.h>

namespace vix::game::sdl
{
  SDLRenderer::~SDLRenderer()
  {
    shutdown();
  }

  GameBoolResult SDLRenderer::init(Window &window)
  {
    if (!window.open())
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "cannot initialize SDL renderer with a closed window");
    }

    auto *native_window = static_cast<SDL_Window *>(window.native_handle());
    if (native_window == nullptr)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "SDL renderer requires an SDL window native handle");
    }

    if (renderer_ != nullptr)
    {
      shutdown();
    }

    renderer_ = SDL_CreateRenderer(
        native_window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer_ == nullptr)
    {
      renderer_ = SDL_CreateRenderer(
          native_window,
          -1,
          SDL_RENDERER_SOFTWARE);
    }

    if (renderer_ == nullptr)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          SDL_GetError());
    }

    width_ = window.width();
    height_ = window.height();

    if (width_ == 0 || height_ == 0)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "SDL renderer size must be greater than zero");
    }

    SDL_RenderSetLogicalSize(
        renderer_,
        static_cast<int>(width_),
        static_cast<int>(height_));

    initialized_ = true;
    frame_active_ = false;
    sprite_count_ = 0;
    uploaded_texture_count_ = 0;

    return true;
  }

  GameBoolResult SDLRenderer::upload_texture(const Asset &asset)
  {
    if (!initialized_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "SDL renderer is not initialized");
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

    SDL_Surface *surface = IMG_Load_RW(rw, 1);
    if (surface == nullptr)
    {
      return make_game_error(
          GameErrorCode::AssetLoadFailed,
          IMG_GetError());
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr)
    {
      return make_game_error(
          GameErrorCode::AssetLoadFailed,
          SDL_GetError());
    }

    textures_.emplace(asset.id(), texture);
    uploaded_texture_count_ = textures_.size();

    return true;
  }

  void SDLRenderer::shutdown() noexcept
  {
    for (auto &[id, texture] : textures_)
    {
      (void)id;

      if (texture != nullptr)
      {
        SDL_DestroyTexture(texture);
      }
    }

    textures_.clear();

    if (renderer_ != nullptr)
    {
      SDL_DestroyRenderer(renderer_);
      renderer_ = nullptr;
    }

    initialized_ = false;
    frame_active_ = false;
    width_ = 0;
    height_ = 0;
    sprite_count_ = 0;
    uploaded_texture_count_ = 0;
  }

  void SDLRenderer::begin_frame()
  {
    if (!initialized_)
    {
      return;
    }

    frame_active_ = true;
    sprite_count_ = 0;
  }

  void SDLRenderer::end_frame()
  {
    if (!initialized_)
    {
      return;
    }

    SDL_RenderPresent(renderer_);
    frame_active_ = false;
  }

  void SDLRenderer::clear(Color color)
  {
    if (!initialized_)
    {
      return;
    }

    const auto clamped = color.clamped();

    SDL_SetRenderDrawColor(
        renderer_,
        to_u8(clamped.r),
        to_u8(clamped.g),
        to_u8(clamped.b),
        to_u8(clamped.a));

    SDL_RenderClear(renderer_);
  }

  void SDLRenderer::draw_sprite(const DrawSpriteCommand &command)
  {
    if (!initialized_ || !frame_active_ || !command.valid())
    {
      return;
    }

    const AssetId id = command.sprite.texture.asset_id();

    const auto it = textures_.find(id);
    if (it == textures_.end() || it->second == nullptr)
    {
      return;
    }

    const auto &transform = command.transform;
    const auto &sprite = command.sprite;

    SDL_Rect src;
    src.x = static_cast<int>(sprite.source.x);
    src.y = static_cast<int>(sprite.source.y);
    src.w = static_cast<int>(sprite.source.width);
    src.h = static_cast<int>(sprite.source.height);

    SDL_Rect dst;
    dst.x = static_cast<int>(transform.position.x - transform.origin.x);
    dst.y = static_cast<int>(transform.position.y - transform.origin.y);
    dst.w = static_cast<int>(sprite.size.x * transform.scale.x);
    dst.h = static_cast<int>(sprite.size.y * transform.scale.y);

    if (src.w <= 0 || src.h <= 0)
    {
      int texture_width = 0;
      int texture_height = 0;

      if (SDL_QueryTexture(
              it->second,
              nullptr,
              nullptr,
              &texture_width,
              &texture_height) != 0)
      {
        return;
      }

      src.x = 0;
      src.y = 0;
      src.w = texture_width;
      src.h = texture_height;

      if (dst.w <= 0)
      {
        dst.w = texture_width;
      }

      if (dst.h <= 0)
      {
        dst.h = texture_height;
      }
    }

    if (dst.w <= 0 || dst.h <= 0)
    {
      return;
    }

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (sprite.flip_x)
    {
      flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_HORIZONTAL);
    }

    if (sprite.flip_y)
    {
      flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);
    }

    SDL_SetTextureColorMod(
        it->second,
        to_u8(sprite.tint.r),
        to_u8(sprite.tint.g),
        to_u8(sprite.tint.b));

    SDL_SetTextureAlphaMod(
        it->second,
        to_u8(sprite.tint.a));

    const double angle_degrees =
        static_cast<double>(transform.rotation) * 57.29577951308232;

    SDL_Point center;
    center.x = static_cast<int>(transform.origin.x);
    center.y = static_cast<int>(transform.origin.y);

    const int rendered = SDL_RenderCopyEx(
        renderer_,
        it->second,
        &src,
        &dst,
        angle_degrees,
        &center,
        flip);

    if (rendered == 0)
    {
      ++sprite_count_;
    }
  }
  void SDLRenderer::resize(
      std::uint32_t width,
      std::uint32_t height)
  {
    if (width == 0 || height == 0)
    {
      return;
    }

    width_ = width;
    height_ = height;

    if (renderer_ != nullptr)
    {
      SDL_RenderSetLogicalSize(
          renderer_,
          static_cast<int>(width_),
          static_cast<int>(height_));
    }
  }

  bool SDLRenderer::initialized() const noexcept
  {
    return initialized_;
  }

  std::uint32_t SDLRenderer::width() const noexcept
  {
    return width_;
  }

  std::uint32_t SDLRenderer::height() const noexcept
  {
    return height_;
  }

  const char *SDLRenderer::backend_name() const noexcept
  {
    return "sdl";
  }

  void *SDLRenderer::native_handle() noexcept
  {
    return renderer_;
  }

  const void *SDLRenderer::native_handle() const noexcept
  {
    return renderer_;
  }

  bool SDLRenderer::frame_active() const noexcept
  {
    return frame_active_;
  }

  std::uint64_t SDLRenderer::sprite_count() const noexcept
  {
    return sprite_count_;
  }

  std::uint64_t SDLRenderer::uploaded_texture_count() const noexcept
  {
    return uploaded_texture_count_;
  }

  std::uint8_t SDLRenderer::to_u8(float value) noexcept
  {
    const float clamped = std::clamp(value, 0.0F, 1.0F);
    return static_cast<std::uint8_t>(clamped * 255.0F);
  }

} // namespace vix::game::sdl
