/**
 *
 *  @file SceneManagerTests.cpp
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

#include <memory>
#include <string>

#include <vix/game/App.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/Frame.hpp>
#include <vix/game/Scene.hpp>
#include <vix/game/SceneManager.hpp>

namespace
{
  class TestScene final : public vix::game::Scene
  {
  public:
    int loaded_count{0};
    int unloaded_count{0};
    int entered_count{0};
    int exited_count{0};
    int update_count{0};
    int fixed_update_count{0};
    bool attached_seen{false};

    vix::game::GameBoolResult on_load() override
    {
      ++loaded_count;
      return vix::game::Scene::on_load();
    }

    vix::game::GameBoolResult on_unload() override
    {
      ++unloaded_count;
      return vix::game::Scene::on_unload();
    }

    void on_enter() override
    {
      ++entered_count;
      vix::game::Scene::on_enter();
    }

    void on_exit() override
    {
      ++exited_count;
      vix::game::Scene::on_exit();
    }

    void on_attach(vix::game::App &app) override
    {
      attached_seen = true;
      vix::game::Scene::on_attach(app);
    }

    void on_update(const vix::game::Frame &frame) override
    {
      (void)frame;
      ++update_count;
    }

    void on_fixed_update(const vix::game::Frame &frame) override
    {
      (void)frame;
      ++fixed_update_count;
    }
  };
} // namespace

TEST(SceneManagerTests, AddSceneAssignsValidIdAndName)
{
  vix::game::App app;
  vix::game::SceneManager manager(app);

  auto id = manager.add("main", std::make_unique<TestScene>());

  ASSERT_TRUE(id);
  EXPECT_TRUE(vix::game::is_valid_scene_id(id.value()));
  EXPECT_TRUE(manager.contains(id.value()));
  EXPECT_TRUE(manager.contains("main"));
  EXPECT_EQ(manager.id_for("main"), id.value());
  EXPECT_EQ(manager.size(), 1u);

  const auto *scene = manager.get(id.value());
  ASSERT_NE(scene, nullptr);
  EXPECT_EQ(scene->name(), "main");
  EXPECT_EQ(scene->id(), id.value());
}

TEST(SceneManagerTests, AddRejectsEmptyName)
{
  vix::game::App app;
  vix::game::SceneManager manager(app);

  auto id = manager.add("", std::make_unique<TestScene>());

  EXPECT_FALSE(id);
  EXPECT_TRUE(manager.empty());
}

TEST(SceneManagerTests, AddRejectsNullScene)
{
  vix::game::App app;
  vix::game::SceneManager manager(app);

  auto id = manager.add("main", nullptr);

  EXPECT_FALSE(id);
  EXPECT_TRUE(manager.empty());
}

TEST(SceneManagerTests, AddRejectsDuplicateName)
{
  vix::game::App app;
  vix::game::SceneManager manager(app);

  auto first = manager.add("main", std::make_unique<TestScene>());
  ASSERT_TRUE(first);

  auto second = manager.add("main", std::make_unique<TestScene>());

  EXPECT_FALSE(second);
  EXPECT_EQ(manager.size(), 1u);
}

TEST(SceneManagerTests, LoadSceneCallsOnLoadOnce)
{
  vix::game::App app;
  vix::game::SceneManager manager(app);

  auto id = manager.add("main", std::make_unique<TestScene>());
  ASSERT_TRUE(id);

  auto loaded = manager.load(id.value());
  ASSERT_TRUE(loaded);
  EXPECT_TRUE(loaded.value());

  auto loaded_again = manager.load(id.value());
  ASSERT_TRUE(loaded_again);
  EXPECT_TRUE(loaded_again.value());

  auto *scene = dynamic_cast<TestScene *>(manager.get(id.value()));
  ASSERT_NE(scene, nullptr);

  EXPECT_TRUE(scene->loaded());
  EXPECT_EQ(scene->loaded_count, 1);
}

TEST(SceneManagerTests, SetActiveLoadsAndEntersScene)
{
  vix::game::App app;
  vix::game::SceneManager manager(app);

  auto id = manager.add("main", std::make_unique<TestScene>());
  ASSERT_TRUE(id);

  auto result = manager.set_active(id.value());

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_EQ(manager.active_id(), id.value());
  EXPECT_EQ(manager.active_name(), "main");

  auto *scene = dynamic_cast<TestScene *>(manager.active());
  ASSERT_NE(scene, nullptr);

  EXPECT_TRUE(scene->loaded());
  EXPECT_TRUE(scene->active());
  EXPECT_EQ(scene->loaded_count, 1);
  EXPECT_EQ(scene->entered_count, 1);
}

TEST(SceneManagerTests, SwitchingActiveSceneExitsPreviousScene)
{
  vix::game::App app;
  vix::game::SceneManager manager(app);

  auto first = manager.add("menu", std::make_unique<TestScene>());
  auto second = manager.add("gameplay", std::make_unique<TestScene>());

  ASSERT_TRUE(first);
  ASSERT_TRUE(second);

  auto first_active = manager.set_active(first.value());
  ASSERT_TRUE(first_active);

  auto second_active = manager.set_active(second.value());
  ASSERT_TRUE(second_active);

  auto *menu = dynamic_cast<TestScene *>(manager.get(first.value()));
  auto *gameplay = dynamic_cast<TestScene *>(manager.get(second.value()));

  ASSERT_NE(menu, nullptr);
  ASSERT_NE(gameplay, nullptr);

  EXPECT_FALSE(menu->active());
  EXPECT_EQ(menu->exited_count, 1);

  EXPECT_TRUE(gameplay->active());
  EXPECT_EQ(gameplay->entered_count, 1);
  EXPECT_EQ(manager.active_id(), second.value());
}

TEST(SceneManagerTests, UpdateForwardsToActiveSceneOnly)
{
  vix::game::App app;
  vix::game::SceneManager manager(app);

  auto first = manager.add("menu", std::make_unique<TestScene>());
  auto second = manager.add("gameplay", std::make_unique<TestScene>());

  ASSERT_TRUE(first);
  ASSERT_TRUE(second);

  auto active = manager.set_active(second.value());
  ASSERT_TRUE(active);

  vix::game::Frame frame;
  manager.update(frame);
  manager.fixed_update(frame);

  auto *menu = dynamic_cast<TestScene *>(manager.get(first.value()));
  auto *gameplay = dynamic_cast<TestScene *>(manager.get(second.value()));

  ASSERT_NE(menu, nullptr);
  ASSERT_NE(gameplay, nullptr);

  EXPECT_EQ(menu->update_count, 0);
  EXPECT_EQ(menu->fixed_update_count, 0);

  EXPECT_EQ(gameplay->update_count, 1);
  EXPECT_EQ(gameplay->fixed_update_count, 1);
}

TEST(SceneManagerTests, RemoveActiveSceneClearsActiveId)
{
  vix::game::App app;
  vix::game::SceneManager manager(app);

  auto id = manager.add("main", std::make_unique<TestScene>());
  ASSERT_TRUE(id);

  auto active = manager.set_active(id.value());
  ASSERT_TRUE(active);

  auto removed = manager.remove(id.value());

  ASSERT_TRUE(removed);
  EXPECT_TRUE(removed.value());
  EXPECT_FALSE(vix::game::is_valid_scene_id(manager.active_id()));
  EXPECT_TRUE(manager.empty());
}

TEST(SceneManagerTests, NamesReturnsRegisteredSceneNames)
{
  vix::game::App app;
  vix::game::SceneManager manager(app);

  ASSERT_TRUE(manager.add("menu", std::make_unique<TestScene>()));
  ASSERT_TRUE(manager.add("gameplay", std::make_unique<TestScene>()));

  auto names = manager.names();

  EXPECT_EQ(names.size(), 2u);
}
