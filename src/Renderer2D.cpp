/**
 *
 *  @file Renderer2D.cpp
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

#include <vix/game/Renderer2D.hpp>

namespace vix::game
{
  Renderer2D::Renderer2D(Renderer &renderer) noexcept
      : renderer_(&renderer),
        camera_(),
        commands_(),
        frame_active_(false)
  {
  }

  Renderer2D &Renderer2D::attach(Renderer &renderer) noexcept
  {
    renderer_ = &renderer;
    return *this;
  }

  void Renderer2D::detach() noexcept
  {
    renderer_ = nullptr;
    frame_active_ = false;
    commands_.clear();
  }

  bool Renderer2D::attached() const noexcept
  {
    return renderer_ != nullptr;
  }

  void Renderer2D::begin_frame()
  {
    commands_.clear();
    frame_active_ = true;

    if (renderer_)
    {
      renderer_->begin_frame();
    }
  }

  void Renderer2D::end_frame()
  {
    if (renderer_)
    {
      for (const auto &command : commands_)
      {
        renderer_->draw_sprite(command);
      }

      renderer_->end_frame();
    }

    frame_active_ = false;
  }

  void Renderer2D::clear(Color color)
  {
    if (renderer_)
    {
      renderer_->clear(color);
    }
  }

  void Renderer2D::draw_sprite(
      const Transform2D &transform,
      const Sprite &sprite)
  {
    draw_sprite(camera_, transform, sprite);
  }

  void Renderer2D::draw_sprite(
      const Camera2D &camera,
      const Transform2D &transform,
      const Sprite &sprite)
  {
    if (!frame_active_)
    {
      return;
    }

    DrawSpriteCommand command;
    command.camera = camera;
    command.transform = transform;
    command.sprite = sprite;

    if (!command.valid())
    {
      return;
    }

    commands_.push_back(command);
  }

  Renderer2D &Renderer2D::set_camera(Camera2D camera) noexcept
  {
    camera_ = camera;
    return *this;
  }

  const Camera2D &Renderer2D::camera() const noexcept
  {
    return camera_;
  }

  bool Renderer2D::frame_active() const noexcept
  {
    return frame_active_;
  }

  const std::vector<DrawSpriteCommand> &Renderer2D::commands() const noexcept
  {
    return commands_;
  }

  std::size_t Renderer2D::command_count() const noexcept
  {
    return commands_.size();
  }

  bool Renderer2D::empty() const noexcept
  {
    return commands_.empty();
  }

  void Renderer2D::clear_commands()
  {
    commands_.clear();
  }

  Renderer *Renderer2D::renderer() noexcept
  {
    return renderer_;
  }

  const Renderer *Renderer2D::renderer() const noexcept
  {
    return renderer_;
  }

} // namespace vix::game
