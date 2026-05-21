/**
 *
 *  @file scene_demo.cpp
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

#include <vix/game/game.hpp>
#include <vix/print.hpp>

#include <cstdint>
#include <memory>

class MainScene final : public vix::game::Scene
{
public:
  vix::game::GameBoolResult on_load() override
  {
    vix::print("scene loaded:", name());
    return vix::game::Scene::on_load();
  }

  void on_enter() override
  {
    vix::game::Scene::on_enter();
    vix::print("scene entered:", name());
  }

  void on_update(const vix::game::Frame &frame) override
  {
    vix::print("scene update frame:", frame.index);

    ++updates_;

    if (updates_ >= 3)
    {
      app().stop();
    }
  }

  void on_exit() override
  {
    vix::print("scene exited:", name());
    vix::game::Scene::on_exit();
  }

  vix::game::GameBoolResult on_unload() override
  {
    vix::print("scene unloaded:", name());
    return vix::game::Scene::on_unload();
  }

private:
  std::uint32_t updates_{0};
};

int main()
{
  vix::game::App app;

  app.set_title("Scene Demo");
  app.set_target_fps(0);

  auto scene_id = app.scenes().add(
      "main",
      std::make_unique<MainScene>());

  if (!scene_id)
  {
    vix::print("error:", scene_id.error().message());
    return 1;
  }

  auto active = app.scenes().set_active(scene_id.value());
  if (!active)
  {
    vix::print("error:", active.error().message());
    return 1;
  }

  auto result = app.run();

  if (!result)
  {
    vix::print("error:", result.error().message());
    return 1;
  }

  return 0;
}
