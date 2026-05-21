/**
 *
 *  @file Entity.hpp
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
#ifndef VIX_GAME_ENTITY_HPP
#define VIX_GAME_ENTITY_HPP

#include <string>
#include <utility>

#include <vix/game/EntityId.hpp>

namespace vix::game
{
  /**
   * @brief Lightweight entity handle.
   *
   * Entity represents a logical object inside a Registry.
   *
   * It does not own components directly. Components are stored and managed by
   * Registry. This keeps Entity small, cheap to copy, and stable as a public
   * identifier object.
   */
  class Entity
  {
  public:
    /**
     * @brief Construct an invalid entity.
     */
    Entity() noexcept = default;

    /**
     * @brief Construct an entity from an id.
     *
     * @param id Entity id.
     */
    explicit Entity(EntityId id) noexcept
        : id_(id)
    {
    }

    /**
     * @brief Construct an entity from an id and a name.
     *
     * @param id Entity id.
     * @param name Entity name.
     */
    Entity(EntityId id, std::string name)
        : id_(id),
          name_(std::move(name))
    {
    }

    /**
     * @brief Return the entity id.
     */
    [[nodiscard]] EntityId id() const noexcept
    {
      return id_;
    }

    /**
     * @brief Set the entity id.
     *
     * Usually called by Registry.
     *
     * @param id Entity id.
     */
    void set_id(EntityId id) noexcept
    {
      id_ = id;
    }

    /**
     * @brief Return true if the entity id is valid.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return is_valid_entity_id(id_);
    }

    /**
     * @brief Return the entity name.
     */
    [[nodiscard]] const std::string &name() const noexcept
    {
      return name_;
    }

    /**
     * @brief Set the entity name.
     *
     * @param value New entity name.
     * @return Reference to this entity.
     */
    Entity &set_name(std::string value)
    {
      name_ = std::move(value);
      return *this;
    }

    /**
     * @brief Return true if the entity has a non-empty name.
     */
    [[nodiscard]] bool has_name() const noexcept
    {
      return !name_.empty();
    }

    /**
     * @brief Compare two entities by id.
     */
    [[nodiscard]] friend bool operator==(const Entity &lhs, const Entity &rhs) noexcept
    {
      return lhs.id_ == rhs.id_;
    }

    /**
     * @brief Compare two entities by id.
     */
    [[nodiscard]] friend bool operator!=(const Entity &lhs, const Entity &rhs) noexcept
    {
      return !(lhs == rhs);
    }

  private:
    /**
     * @brief Entity identifier.
     */
    EntityId id_{invalid_entity_id};

    /**
     * @brief Optional human-readable entity name.
     */
    std::string name_{};
  };

} // namespace vix::game

#endif // VIX_GAME_ENTITY_HPP
