/**
 *
 *  @file PhysicsRuntime.cpp
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

#include <vix/game/PhysicsRuntime.hpp>
#include <vix/game/GameError.hpp>

namespace vix::game
{
  PhysicsRuntime::PhysicsRuntime(GameContext &context) noexcept
      : context_(&context),
        bodies_(),
        gravity_(0.0F, 9.81F),
        initialized_(false)
  {
  }

  PhysicsRuntime &PhysicsRuntime::attach(GameContext &context) noexcept
  {
    context_ = &context;
    return *this;
  }

  void PhysicsRuntime::detach() noexcept
  {
    shutdown();
    context_ = nullptr;
  }

  bool PhysicsRuntime::attached() const noexcept
  {
    return context_ != nullptr;
  }

  GameContext &PhysicsRuntime::context()
  {
    return *context_;
  }

  const GameContext &PhysicsRuntime::context() const
  {
    return *context_;
  }

  GameBoolResult PhysicsRuntime::init()
  {
    if (initialized_)
    {
      return true;
    }

    if (!attached())
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "physics runtime is not attached to a game context");
    }

    initialized_ = true;
    return true;
  }

  void PhysicsRuntime::shutdown() noexcept
  {
    if (!initialized_)
    {
      return;
    }

    clear();
    initialized_ = false;
  }

  bool PhysicsRuntime::initialized() const noexcept
  {
    return initialized_;
  }

  GameBoolResult PhysicsRuntime::add_body(
      EntityId entity,
      Vec2 position,
      Vec2 velocity)
  {
    if (!is_valid_entity_id(entity))
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "physics body entity id is invalid");
    }

    if (has_body(entity))
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "physics body already exists");
    }

    Body item;
    item.entity = entity;
    item.position = position;
    item.velocity = velocity;
    item.acceleration = {};
    item.mass = 1.0F;
    item.dynamic = true;

    bodies_.emplace(entity, item);

    return true;
  }

  GameBoolResult PhysicsRuntime::remove_body(EntityId entity)
  {
    const auto erased = bodies_.erase(entity);
    if (erased == 0)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "physics body not found");
    }

    return true;
  }

  bool PhysicsRuntime::has_body(EntityId entity) const
  {
    return bodies_.find(entity) != bodies_.end();
  }

  PhysicsRuntime::Body *PhysicsRuntime::body(EntityId entity)
  {
    const auto it = bodies_.find(entity);
    if (it == bodies_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  const PhysicsRuntime::Body *PhysicsRuntime::body(EntityId entity) const
  {
    const auto it = bodies_.find(entity);
    if (it == bodies_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  GameBoolResult PhysicsRuntime::set_position(EntityId entity, Vec2 position)
  {
    auto *item = body(entity);
    if (!item)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "physics body not found");
    }

    item->position = position;
    return true;
  }

  GameBoolResult PhysicsRuntime::set_velocity(EntityId entity, Vec2 velocity)
  {
    auto *item = body(entity);
    if (!item)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "physics body not found");
    }

    item->velocity = velocity;
    return true;
  }

  GameBoolResult PhysicsRuntime::apply_force(EntityId entity, Vec2 force)
  {
    auto *item = body(entity);
    if (!item)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "physics body not found");
    }

    if (!item->dynamic)
    {
      return true;
    }

    item->acceleration.x += force.x / item->mass;
    item->acceleration.y += force.y / item->mass;

    return true;
  }

  void PhysicsRuntime::fixed_update(const Frame &frame)
  {
    const float dt = static_cast<float>(frame.delta_ms()) / 1000.0F;

    for (auto &[entity, item] : bodies_)
    {
      (void)entity;

      if (!item.dynamic)
      {
        continue;
      }

      item.acceleration.x += gravity_.x;
      item.acceleration.y += gravity_.y;

      item.velocity.x += item.acceleration.x * dt;
      item.velocity.y += item.acceleration.y * dt;

      item.position.x += item.velocity.x * dt;
      item.position.y += item.velocity.y * dt;

      item.acceleration = {};
    }
  }

  void PhysicsRuntime::clear()
  {
    bodies_.clear();
  }

  std::size_t PhysicsRuntime::size() const noexcept
  {
    return bodies_.size();
  }

  bool PhysicsRuntime::empty() const noexcept
  {
    return bodies_.empty();
  }

  PhysicsRuntime &PhysicsRuntime::set_gravity(Vec2 gravity) noexcept
  {
    gravity_ = gravity;
    return *this;
  }

  Vec2 PhysicsRuntime::gravity() const noexcept
  {
    return gravity_;
  }

} // namespace vix::game
