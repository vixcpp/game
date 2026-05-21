/**
 *
 *  @file Renderer.cpp
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

#include <vix/game/Renderer.hpp>

#include <utility>

#include <vix/game/GameError.hpp>

namespace vix::game
{
  Renderer::Renderer(std::unique_ptr<RendererBackend> backend) noexcept
      : backend_(std::move(backend))
  {
  }

  Renderer::~Renderer()
  {
    shutdown();
  }

  Renderer &Renderer::set_backend(
      std::unique_ptr<RendererBackend> backend) noexcept
  {
    shutdown();
    backend_ = std::move(backend);
    return *this;
  }

  bool Renderer::has_backend() const noexcept
  {
    return backend_ != nullptr;
  }

  GameBoolResult Renderer::init(Window &window)
  {
    if (!backend_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "renderer backend is not set");
    }

    return backend_->init(window);
  }

  void Renderer::shutdown() noexcept
  {
    if (backend_)
    {
      backend_->shutdown();
    }
  }

  void Renderer::begin_frame()
  {
    if (backend_)
    {
      backend_->begin_frame();
    }
  }

  void Renderer::end_frame()
  {
    if (backend_)
    {
      backend_->end_frame();
    }
  }

  void Renderer::clear(Color color)
  {
    if (backend_)
    {
      backend_->clear(color);
    }
  }

  void Renderer::resize(
      std::uint32_t width,
      std::uint32_t height)
  {
    if (backend_)
    {
      backend_->resize(width, height);
    }
  }

  void Renderer::execute(const RenderCommand &command)
  {
    if (!command.valid())
    {
      return;
    }

    switch (command.type)
    {
    case RenderCommandType::BeginFrame:
      begin_frame();
      break;

    case RenderCommandType::EndFrame:
      end_frame();
      break;

    case RenderCommandType::Clear:
      clear(command.color);
      break;

    case RenderCommandType::Resize:
      resize(command.width, command.height);
      break;

    case RenderCommandType::Unknown:
      break;
    }
  }

  bool Renderer::initialized() const noexcept
  {
    return backend_ != nullptr && backend_->initialized();
  }

  std::uint32_t Renderer::width() const noexcept
  {
    if (!backend_)
    {
      return 0;
    }

    return backend_->width();
  }

  std::uint32_t Renderer::height() const noexcept
  {
    if (!backend_)
    {
      return 0;
    }

    return backend_->height();
  }

  const char *Renderer::backend_name() const noexcept
  {
    if (!backend_)
    {
      return "none";
    }

    return backend_->backend_name();
  }

  void *Renderer::native_handle() noexcept
  {
    if (!backend_)
    {
      return nullptr;
    }

    return backend_->native_handle();
  }

  const void *Renderer::native_handle() const noexcept
  {
    if (!backend_)
    {
      return nullptr;
    }

    return backend_->native_handle();
  }

  RendererBackend *Renderer::backend() noexcept
  {
    return backend_.get();
  }

  const RendererBackend *Renderer::backend() const noexcept
  {
    return backend_.get();
  }

} // namespace vix::game
