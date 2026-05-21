/**
 *
 *  @file Registry.hpp
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
#ifndef VIX_GAME_REGISTRY_HPP
#define VIX_GAME_REGISTRY_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include <vix/game/Component.hpp>
#include <vix/game/Entity.hpp>
#include <vix/game/EntityId.hpp>
#include <vix/game/Frame.hpp>
#include <vix/game/GameError.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/System.hpp>

namespace vix::game
{
  /**
   * @brief Stores entities, components, and systems.
   *
   * Registry is the V1 ECS-style container used by vix/game.
   *
   * Design:
   * - entities are lightweight handles
   * - components are stored by C++ type
   * - systems are owned by the registry
   * - updates are forwarded to systems
   *
   * This is intentionally simple. It is not yet a high-performance archetype
   * ECS. It is a clean foundation that can evolve later.
   */
  class Registry
  {
  public:
    /**
     * @brief Construct an empty registry.
     */
    Registry() = default;

    Registry(const Registry &) = delete;
    Registry &operator=(const Registry &) = delete;

    /**
     * @brief Destroy the registry.
     */
    ~Registry() = default;

    /**
     * @brief Create an unnamed entity.
     *
     * @return Created entity.
     */
    [[nodiscard]] Entity create_entity()
    {
      return create_entity({});
    }

    /**
     * @brief Create a named entity.
     *
     * @param name Entity name.
     * @return Created entity.
     */
    [[nodiscard]] Entity create_entity(std::string name)
    {
      const EntityId id = next_entity_id_++;

      Entity entity(id, std::move(name));
      entities_.emplace(id, entity);

      return entity;
    }

    /**
     * @brief Destroy an entity and all of its components.
     *
     * @param id Entity id.
     * @return true if the entity existed and was destroyed.
     */
    [[nodiscard]] GameBoolResult destroy_entity(EntityId id)
    {
      if (!is_valid_entity_id(id))
      {
        return make_game_error(
            GameErrorCode::InvalidArgument,
            "entity id is invalid");
      }

      const auto erased = entities_.erase(id);
      if (erased == 0)
      {
        return make_game_error(
            GameErrorCode::EntityNotFound,
            "entity not found");
      }

      for (auto &[type, storage] : component_stores_)
      {
        (void)type;
        storage->remove(id);
      }

      return true;
    }

    /**
     * @brief Return true if an entity exists.
     *
     * @param id Entity id.
     * @return true if found.
     */
    [[nodiscard]] bool has_entity(EntityId id) const
    {
      return entities_.find(id) != entities_.end();
    }

    /**
     * @brief Return an entity by id.
     *
     * @param id Entity id.
     * @return Entity pointer, or nullptr if missing.
     */
    [[nodiscard]] Entity *entity(EntityId id)
    {
      const auto it = entities_.find(id);
      if (it == entities_.end())
      {
        return nullptr;
      }

      return &it->second;
    }

    /**
     * @brief Return an entity by id.
     *
     * @param id Entity id.
     * @return Entity pointer, or nullptr if missing.
     */
    [[nodiscard]] const Entity *entity(EntityId id) const
    {
      const auto it = entities_.find(id);
      if (it == entities_.end())
      {
        return nullptr;
      }

      return &it->second;
    }

    /**
     * @brief Add or replace a component for an entity.
     *
     * @tparam T Component type.
     * @tparam Args Constructor argument types.
     * @param id Entity id.
     * @param args Component constructor arguments.
     * @return Reference to the stored component, or an error.
     */
    template <typename T, typename... Args>
    [[nodiscard]] GameResult<T *> emplace_component(EntityId id, Args &&...args)
    {
      if (!has_entity(id))
      {
        return make_game_error(
            GameErrorCode::EntityNotFound,
            "cannot add component to missing entity");
      }

      auto &store = component_store<T>();
      T &component = store.emplace(id, std::forward<Args>(args)...);

      return &component;
    }

    /**
     * @brief Return true if an entity owns a component of type T.
     *
     * @tparam T Component type.
     * @param id Entity id.
     * @return true if the component exists.
     */
    template <typename T>
    [[nodiscard]] bool has_component(EntityId id) const
    {
      const auto *store = find_component_store<T>();
      return store != nullptr && store->contains(id);
    }

    /**
     * @brief Get a component of type T.
     *
     * @tparam T Component type.
     * @param id Entity id.
     * @return Component pointer, or nullptr if missing.
     */
    template <typename T>
    [[nodiscard]] T *get_component(EntityId id)
    {
      auto *store = find_component_store<T>();
      if (store == nullptr)
      {
        return nullptr;
      }

      return store->get(id);
    }

    /**
     * @brief Get a component of type T.
     *
     * @tparam T Component type.
     * @param id Entity id.
     * @return Component pointer, or nullptr if missing.
     */
    template <typename T>
    [[nodiscard]] const T *get_component(EntityId id) const
    {
      const auto *store = find_component_store<T>();
      if (store == nullptr)
      {
        return nullptr;
      }

      return store->get(id);
    }

    /**
     * @brief Remove a component of type T from an entity.
     *
     * @tparam T Component type.
     * @param id Entity id.
     * @return true if the component was removed.
     */
    template <typename T>
    [[nodiscard]] GameBoolResult remove_component(EntityId id)
    {
      auto *store = find_component_store<T>();
      if (store == nullptr || !store->contains(id))
      {
        return make_game_error(
            GameErrorCode::ComponentNotFound,
            "component not found");
      }

      store->remove(id);
      return true;
    }

    /**
     * @brief Add a system instance.
     *
     * @param system System instance.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult add_system(std::unique_ptr<System> system)
    {
      if (!system)
      {
        return make_game_error(
            GameErrorCode::InvalidArgument,
            "system cannot be null");
      }

      system->on_attach(*this);

      auto started = system->on_start();
      if (!started)
      {
        system->on_detach();
        return started.error();
      }

      systems_.push_back(std::move(system));
      return true;
    }

    /**
     * @brief Create and add a system.
     *
     * @tparam T System type.
     * @tparam Args Constructor argument types.
     * @param args Constructor arguments.
     * @return true on success, or a structured error.
     */
    template <typename T, typename... Args>
    [[nodiscard]] GameBoolResult create_system(Args &&...args)
    {
      auto system = std::make_unique<T>(std::forward<Args>(args)...);
      return add_system(std::move(system));
    }

    /**
     * @brief Update all systems.
     *
     * @param frame Current frame.
     */
    void update(const Frame &frame)
    {
      for (auto &system : systems_)
      {
        if (system)
        {
          system->on_update(frame);
        }
      }
    }

    /**
     * @brief Fixed update all systems.
     *
     * @param frame Current frame.
     */
    void fixed_update(const Frame &frame)
    {
      for (auto &system : systems_)
      {
        if (system)
        {
          system->on_fixed_update(frame);
        }
      }
    }

    /**
     * @brief Remove all entities, components, and systems.
     */
    void clear()
    {
      for (auto &system : systems_)
      {
        if (system)
        {
          system->on_stop();
          system->on_detach();
        }
      }

      systems_.clear();
      component_stores_.clear();
      entities_.clear();
      next_entity_id_ = 1;
    }

    /**
     * @brief Return entity count.
     */
    [[nodiscard]] std::size_t entity_count() const noexcept
    {
      return entities_.size();
    }

    /**
     * @brief Return system count.
     */
    [[nodiscard]] std::size_t system_count() const noexcept
    {
      return systems_.size();
    }

    /**
     * @brief Return true if the registry contains no entities.
     */
    [[nodiscard]] bool empty() const noexcept
    {
      return entities_.empty();
    }

    /**
     * @brief Return all entities.
     */
    [[nodiscard]] std::vector<Entity> entities() const
    {
      std::vector<Entity> out;
      out.reserve(entities_.size());

      for (const auto &[id, entity] : entities_)
      {
        (void)id;
        out.push_back(entity);
      }

      return out;
    }

  private:
    /**
     * @brief Type-erased component storage base.
     */
    class ComponentStoreBase
    {
    public:
      virtual ~ComponentStoreBase() = default;

      /**
       * @brief Remove an entity component from this store.
       */
      virtual void remove(EntityId id) = 0;
    };

    /**
     * @brief Component storage for one concrete component type.
     *
     * @tparam T Component type.
     */
    template <typename T>
    class ComponentStore final : public ComponentStoreBase
    {
    public:
      /**
       * @brief Add or replace a component.
       */
      template <typename... Args>
      T &emplace(EntityId id, Args &&...args)
      {
        auto [it, inserted] = components_.try_emplace(
            id,
            std::forward<Args>(args)...);

        if (!inserted)
        {
          it->second = T(std::forward<Args>(args)...);
        }

        return it->second;
      }

      /**
       * @brief Return true if the component exists.
       */
      [[nodiscard]] bool contains(EntityId id) const
      {
        return components_.find(id) != components_.end();
      }

      /**
       * @brief Return component pointer.
       */
      [[nodiscard]] T *get(EntityId id)
      {
        const auto it = components_.find(id);
        if (it == components_.end())
        {
          return nullptr;
        }

        return &it->second;
      }

      /**
       * @brief Return component pointer.
       */
      [[nodiscard]] const T *get(EntityId id) const
      {
        const auto it = components_.find(id);
        if (it == components_.end())
        {
          return nullptr;
        }

        return &it->second;
      }

      /**
       * @brief Remove component by entity id.
       */
      void remove(EntityId id) override
      {
        components_.erase(id);
      }

      /**
       * @brief Return component count.
       */
      [[nodiscard]] std::size_t size() const noexcept
      {
        return components_.size();
      }

    private:
      /**
       * @brief Components by entity id.
       */
      std::unordered_map<EntityId, T> components_{};
    };

    /**
     * @brief Return or create component store for T.
     */
    template <typename T>
    [[nodiscard]] ComponentStore<T> &component_store()
    {
      const ComponentTypeId type = component_type_id<T>();

      auto it = component_stores_.find(type);
      if (it == component_stores_.end())
      {
        auto store = std::make_unique<ComponentStore<T>>();
        auto *raw = store.get();

        component_stores_.emplace(type, std::move(store));
        return *raw;
      }

      return *static_cast<ComponentStore<T> *>(it->second.get());
    }

    /**
     * @brief Return component store for T if it exists.
     */
    template <typename T>
    [[nodiscard]] ComponentStore<T> *find_component_store()
    {
      const ComponentTypeId type = component_type_id<T>();

      const auto it = component_stores_.find(type);
      if (it == component_stores_.end())
      {
        return nullptr;
      }

      return static_cast<ComponentStore<T> *>(it->second.get());
    }

    /**
     * @brief Return component store for T if it exists.
     */
    template <typename T>
    [[nodiscard]] const ComponentStore<T> *find_component_store() const
    {
      const ComponentTypeId type = component_type_id<T>();

      const auto it = component_stores_.find(type);
      if (it == component_stores_.end())
      {
        return nullptr;
      }

      return static_cast<const ComponentStore<T> *>(it->second.get());
    }

  private:
    /**
     * @brief Entity map.
     */
    std::unordered_map<EntityId, Entity> entities_{};

    /**
     * @brief Type-erased component stores.
     */
    std::unordered_map<ComponentTypeId, std::unique_ptr<ComponentStoreBase>> component_stores_{};

    /**
     * @brief Owned systems.
     */
    std::vector<std::unique_ptr<System>> systems_{};

    /**
     * @brief Next entity id.
     */
    EntityId next_entity_id_{1};
  };

} // namespace vix::game

#endif // VIX_GAME_REGISTRY_HPP
