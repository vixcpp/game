/**
 *
 *  @file PhysicsRuntime.hpp
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
#ifndef VIX_GAME_PHYSICS_RUNTIME_HPP
#define VIX_GAME_PHYSICS_RUNTIME_HPP

#include <cstddef>
#include <unordered_map>

#include <vix/game/EntityId.hpp>
#include <vix/game/Frame.hpp>
#include <vix/game/GameContext.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/Vec2.hpp>

namespace vix::game
{
  /**
   * @brief Runtime foundation for simple 2D physics.
   *
   * PhysicsRuntime is a dependency-free physics layer.
   *
   * It does not try to replace a real physics engine. It provides a stable
   * foundation where future Box2D/Bullet/custom backends can be integrated.
   */
  class PhysicsRuntime
  {
  public:
    /**
     * @brief Basic 2D physics body.
     */
    struct Body
    {
      EntityId entity{invalid_entity_id};
      Vec2 position{};
      Vec2 velocity{};
      Vec2 acceleration{};
      float mass{1.0F};
      bool dynamic{true};

      [[nodiscard]] bool valid() const noexcept
      {
        return is_valid_entity_id(entity) && mass > 0.0F;
      }
    };

    PhysicsRuntime() = default;

    explicit PhysicsRuntime(GameContext &context) noexcept;

    PhysicsRuntime(const PhysicsRuntime &) = delete;
    PhysicsRuntime &operator=(const PhysicsRuntime &) = delete;

    ~PhysicsRuntime() = default;

    PhysicsRuntime &attach(GameContext &context) noexcept;

    void detach() noexcept;

    [[nodiscard]] bool attached() const noexcept;

    [[nodiscard]] GameContext &context();

    [[nodiscard]] const GameContext &context() const;

    [[nodiscard]] GameBoolResult init();

    void shutdown() noexcept;

    [[nodiscard]] bool initialized() const noexcept;

    [[nodiscard]] GameBoolResult add_body(
        EntityId entity,
        Vec2 position = {},
        Vec2 velocity = {});

    [[nodiscard]] GameBoolResult remove_body(EntityId entity);

    [[nodiscard]] bool has_body(EntityId entity) const;

    [[nodiscard]] Body *body(EntityId entity);

    [[nodiscard]] const Body *body(EntityId entity) const;

    [[nodiscard]] GameBoolResult set_position(EntityId entity, Vec2 position);

    [[nodiscard]] GameBoolResult set_velocity(EntityId entity, Vec2 velocity);

    [[nodiscard]] GameBoolResult apply_force(EntityId entity, Vec2 force);

    void fixed_update(const Frame &frame);

    void clear();

    [[nodiscard]] std::size_t size() const noexcept;

    [[nodiscard]] bool empty() const noexcept;

    PhysicsRuntime &set_gravity(Vec2 gravity) noexcept;

    [[nodiscard]] Vec2 gravity() const noexcept;

  private:
    GameContext *context_{nullptr};
    std::unordered_map<EntityId, Body> bodies_{};
    Vec2 gravity_{0.0F, 9.81F};
    bool initialized_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_PHYSICS_RUNTIME_HPP
