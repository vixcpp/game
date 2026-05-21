/**
 *
 *  @file Render2DSystem.cpp
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

#include <vix/game/Render2DSystem.hpp>

#include <vix/game/Registry.hpp>
#include <vix/game/RegistryView.hpp>

namespace vix::game
{
  Render2DSystem::Render2DSystem(Renderer2D &renderer) noexcept
      : System("render_2d"),
        renderer_(&renderer)
  {
  }

  Render2DSystem &Render2DSystem::set_renderer(Renderer2D &renderer) noexcept
  {
    renderer_ = &renderer;
    return *this;
  }

  void Render2DSystem::clear_renderer() noexcept
  {
    renderer_ = nullptr;
  }

  bool Render2DSystem::has_renderer() const noexcept
  {
    return renderer_ != nullptr;
  }

  void Render2DSystem::on_update(const Frame &frame)
  {
    (void)frame;

    if (!renderer_)
    {
      return;
    }

    if (!attached())
    {
      return;
    }

    registry().view<Transform2D, Sprite>().each(
        [this](Entity entity, Transform2D &transform, Sprite &sprite)
        {
          (void)entity;

          if (!sprite.renderable())
          {
            return;
          }

          renderer_->draw_sprite(transform, sprite);
        });
  }

} // namespace vix::game
