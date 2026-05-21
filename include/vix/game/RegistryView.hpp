/**
 *
 *  @file RegistryView.hpp
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
#ifndef VIX_GAME_REGISTRY_VIEW_HPP
#define VIX_GAME_REGISTRY_VIEW_HPP

#include <tuple>
#include <utility>

#include <vix/game/Entity.hpp>
#include <vix/game/EntityId.hpp>

namespace vix::game
{
  class Registry;

  /**
   * @brief Lightweight typed view over entities owning a set of components.
   *
   * RegistryView does not own data.
   * It simply references a Registry and provides iteration helpers.
   *
   * A view like:
   *
   *   RegistryView<Transform, Sprite>
   *
   * iterates only over entities that own both Transform and Sprite.
   *
   * @tparam Components Component types required by this view.
   */
  template <typename... Components>
  class RegistryView
  {
  public:
    /**
     * @brief Construct a view from a registry.
     *
     * @param registry Registry to inspect.
     */
    explicit RegistryView(Registry &registry) noexcept
        : registry_(&registry)
    {
    }

    RegistryView(const RegistryView &) = default;
    RegistryView &operator=(const RegistryView &) = default;

    /**
     * @brief Return true if the view references a registry.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return registry_ != nullptr;
    }

    /**
     * @brief Execute a function for every matching entity.
     *
     * The callback receives:
     * - Entity
     * - Components&...
     *
     * Example:
     *
     *   registry.view<Transform, Sprite>().each(
     *       [](Entity entity, Transform& transform, Sprite& sprite)
     *       {
     *       });
     *
     * @tparam Fn Callback type.
     * @param fn Callback invoked for every matching entity.
     */
    template <typename Fn>
    void each(Fn &&fn)
    {
      if (!registry_)
      {
        return;
      }

      auto entities = registry_->entities();

      for (const auto &entity : entities)
      {
        if (!entity.valid())
        {
          continue;
        }

        const EntityId id = entity.id();

        if (!matches(id))
        {
          continue;
        }

        call(entity, std::forward<Fn>(fn));
      }
    }

    /**
     * @brief Count entities matching this view.
     *
     * @return Number of entities that own all requested components.
     */
    [[nodiscard]] std::size_t count() const
    {
      if (!registry_)
      {
        return 0;
      }

      std::size_t total = 0;
      auto entities = registry_->entities();

      for (const auto &entity : entities)
      {
        if (!entity.valid())
        {
          continue;
        }

        if (matches(entity.id()))
        {
          ++total;
        }
      }

      return total;
    }

    /**
     * @brief Return true if no entity matches this view.
     */
    [[nodiscard]] bool empty() const
    {
      return count() == 0;
    }

  private:
    /**
     * @brief Return true if the entity owns every required component.
     *
     * @param id Entity id.
     * @return true if all components exist.
     */
    [[nodiscard]] bool matches(EntityId id) const
    {
      return (registry_->template has_component<Components>(id) && ...);
    }

    /**
     * @brief Invoke a callback with the entity and its components.
     *
     * @tparam Fn Callback type.
     * @param entity Entity handle.
     * @param fn Callback to invoke.
     */
    template <typename Fn>
    void call(const Entity &entity, Fn &&fn)
    {
      const EntityId id = entity.id();

      std::forward<Fn>(fn)(
          entity,
          *registry_->template get_component<Components>(id)...);
    }

  private:
    /**
     * @brief Registry referenced by this view.
     */
    Registry *registry_{nullptr};
  };

} // namespace vix::game

#endif // VIX_GAME_REGISTRY_VIEW_HPP
