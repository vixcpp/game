/**
 *
 *  @file RegistryTests.cpp
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

#include <string>

#include <vix/game/Frame.hpp>
#include <vix/game/Registry.hpp>
#include <vix/game/System.hpp>

namespace
{
  struct Position
  {
    float x{0.0F};
    float y{0.0F};
  };

  struct Velocity
  {
    float dx{0.0F};
    float dy{0.0F};
  };

  class CountingSystem final : public vix::game::System
  {
  public:
    int started_count{0};
    int stopped_count{0};
    int update_count{0};
    int fixed_update_count{0};
    bool attached_seen{false};

    vix::game::GameBoolResult on_start() override
    {
      ++started_count;
      return vix::game::System::on_start();
    }

    void on_stop() override
    {
      ++stopped_count;
      vix::game::System::on_stop();
    }

    void on_attach(vix::game::Registry &registry) override
    {
      attached_seen = true;
      vix::game::System::on_attach(registry);
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

TEST(RegistryTests, CreateEntityReturnsValidEntity)
{
  vix::game::Registry registry;

  auto entity = registry.create_entity("player");

  EXPECT_TRUE(entity.valid());
  EXPECT_TRUE(registry.has_entity(entity.id()));
  EXPECT_EQ(entity.name(), "player");
  EXPECT_EQ(registry.entity_count(), 1u);
}

TEST(RegistryTests, EntityCanBeRetrievedById)
{
  vix::game::Registry registry;

  auto entity = registry.create_entity("player");

  auto *found = registry.entity(entity.id());

  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->id(), entity.id());
  EXPECT_EQ(found->name(), "player");
}

TEST(RegistryTests, DestroyEntityRemovesEntity)
{
  vix::game::Registry registry;

  auto entity = registry.create_entity("player");

  auto result = registry.destroy_entity(entity.id());

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_FALSE(registry.has_entity(entity.id()));
  EXPECT_EQ(registry.entity_count(), 0u);
}

TEST(RegistryTests, DestroyMissingEntityReturnsError)
{
  vix::game::Registry registry;

  auto result = registry.destroy_entity(42);

  EXPECT_FALSE(result);
}

TEST(RegistryTests, EmplaceAndGetComponent)
{
  vix::game::Registry registry;

  auto entity = registry.create_entity("player");

  auto component = registry.emplace_component<Position>(entity.id(), 10.0F, 20.0F);

  ASSERT_TRUE(component);
  ASSERT_NE(component.value(), nullptr);

  auto *position = registry.get_component<Position>(entity.id());

  ASSERT_NE(position, nullptr);
  EXPECT_FLOAT_EQ(position->x, 10.0F);
  EXPECT_FLOAT_EQ(position->y, 20.0F);
  EXPECT_TRUE(registry.has_component<Position>(entity.id()));
}

TEST(RegistryTests, EmplaceComponentForMissingEntityReturnsError)
{
  vix::game::Registry registry;

  auto component = registry.emplace_component<Position>(42, 10.0F, 20.0F);

  EXPECT_FALSE(component);
}

TEST(RegistryTests, RemoveComponentDeletesComponent)
{
  vix::game::Registry registry;

  auto entity = registry.create_entity("player");

  auto component = registry.emplace_component<Position>(entity.id(), 10.0F, 20.0F);
  ASSERT_TRUE(component);

  auto removed = registry.remove_component<Position>(entity.id());

  ASSERT_TRUE(removed);
  EXPECT_TRUE(removed.value());
  EXPECT_FALSE(registry.has_component<Position>(entity.id()));
  EXPECT_EQ(registry.get_component<Position>(entity.id()), nullptr);
}

TEST(RegistryTests, DestroyEntityRemovesAllComponents)
{
  vix::game::Registry registry;

  auto entity = registry.create_entity("player");

  ASSERT_TRUE(registry.emplace_component<Position>(entity.id(), 10.0F, 20.0F));
  ASSERT_TRUE(registry.emplace_component<Velocity>(entity.id(), 1.0F, 2.0F));

  auto destroyed = registry.destroy_entity(entity.id());

  ASSERT_TRUE(destroyed);
  EXPECT_FALSE(registry.has_component<Position>(entity.id()));
  EXPECT_FALSE(registry.has_component<Velocity>(entity.id()));
}

TEST(RegistryTests, AddSystemStartsAndUpdatesSystem)
{
  vix::game::Registry registry;

  auto system = std::make_unique<CountingSystem>();
  auto *raw = system.get();

  auto result = registry.add_system(std::move(system));

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_TRUE(raw->attached_seen);
  EXPECT_TRUE(raw->started());
  EXPECT_EQ(raw->started_count, 1);
  EXPECT_EQ(registry.system_count(), 1u);

  vix::game::Frame frame;
  registry.update(frame);
  registry.fixed_update(frame);

  EXPECT_EQ(raw->update_count, 1);
  EXPECT_EQ(raw->fixed_update_count, 1);
}

TEST(RegistryTests, AddNullSystemReturnsError)
{
  vix::game::Registry registry;

  auto result = registry.add_system(nullptr);

  EXPECT_FALSE(result);
  EXPECT_EQ(registry.system_count(), 0u);
}

TEST(RegistryTests, ClearRemovesEverything)
{
  vix::game::Registry registry;

  auto entity = registry.create_entity("player");
  ASSERT_TRUE(registry.emplace_component<Position>(entity.id(), 10.0F, 20.0F));

  auto system = std::make_unique<CountingSystem>();
  auto *raw = system.get();

  ASSERT_TRUE(registry.add_system(std::move(system)));

  registry.clear();

  EXPECT_EQ(registry.entity_count(), 0u);
  EXPECT_EQ(registry.system_count(), 0u);
  EXPECT_TRUE(registry.empty());
  EXPECT_EQ(raw->stopped_count, 1);
}

TEST(RegistryTests, EntitiesReturnsEntityList)
{
  vix::game::Registry registry;

  registry.create_entity("a");
  registry.create_entity("b");

  auto entities = registry.entities();

  EXPECT_EQ(entities.size(), 2u);
}
