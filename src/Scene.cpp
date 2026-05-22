/**
 *
 *  @file Scene.cpp
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

#include <vix/game/Scene.hpp>

#include <vix/game/App.hpp>
#include <vix/game/AssetManager.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/JobSystem.hpp>
#include <vix/game/GameContext.hpp>
#include <vix/game/GameRuntime.hpp>
#include <vix/game/InputSystem.hpp>
#include <vix/game/Renderer.hpp>
#include <vix/game/Renderer2D.hpp>
#include <vix/game/Window.hpp>

namespace vix::game
{
  EventBus &Scene::events()
  {
    return app().events();
  }

  AssetManager &Scene::assets()
  {
    return app().assets();
  }

  JobSystem &Scene::jobs()
  {
    return app().jobs();
  }

  GameContext &Scene::context()
  {
    return app().runtime().context();
  }

  InputSystem &Scene::input()
  {
    return context().input();
  }

  Window &Scene::window()
  {
    return context().window();
  }

  Renderer &Scene::renderer()
  {
    return context().renderer();
  }

  Renderer2D &Scene::renderer2d()
  {
    return context().renderer2d();
  }

} // namespace vix::game
