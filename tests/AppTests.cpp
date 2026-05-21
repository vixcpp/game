/**
 *
 *  @file AppTests.cpp
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

#include <gtest/gtest.h>

#include <vix/game/App.hpp>
#include <vix/game/AppConfig.hpp>
#include <vix/game/Frame.hpp>
#include <vix/game/Scene.hpp>
#include <vix/game/SceneManager.hpp>

namespace
{
  class TestScene final : public vix::game::Scene
  {
  public:
    explicit TestScene(int *updates)
        : updates_(updates)
    {
    }

    void on_update(const vix::game::Frame &frame) override
    {
      (void)frame;

      if (updates_)
      {
        ++(*updates_);
      }

      app().stop();
    }

  private:
    int *updates_{nullptr};
  };
} // namespace

TEST(GameAppTests, DefaultAppCanInitialize)
{
  vix::game::App app;

  auto result = app.init();

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_TRUE(app.initialized());

  app.shutdown();
  EXPECT_FALSE(app.initialized());
}

TEST(GameAppTests, AppCanSetTitleBeforeInitialization)
{
  vix::game::App app;

  app.set_title("Test Game");

  EXPECT_EQ(app.config().title, "Test Game");
}

TEST(GameAppTests, AppCanSetTargetFpsBeforeRun)
{
  vix::game::App app;

  app.set_target_fps(30);

  EXPECT_EQ(app.config().time_step.target_fps, 30u);
  EXPECT_TRUE(app.config().time_step.limit_fps);
}

TEST(GameAppTests, AppRejectsConfigChangeAfterInitialization)
{
  vix::game::App app;

  auto init_result = app.init();
  ASSERT_TRUE(init_result);

  vix::game::AppConfig config;
  config.title = "New Title";

  auto result = app.set_config(config);

  EXPECT_FALSE(result);
}

TEST(GameAppTests, AppRunsRegisteredScene)
{
  vix::game::App app;
  int updates = 0;

  auto scene_id = app.scenes().add(
      "main",
      std::make_unique<TestScene>(&updates));

  ASSERT_TRUE(scene_id);

  auto active = app.scenes().set_active(scene_id.value());
  ASSERT_TRUE(active);

  auto result = app.run();

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_EQ(updates, 1);
}
