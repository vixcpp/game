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

} // namespace vix::game
