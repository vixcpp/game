/**
 *
 *  @file Component.hpp
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
#ifndef VIX_GAME_COMPONENT_HPP
#define VIX_GAME_COMPONENT_HPP

#include <cstdint>
#include <string_view>
#include <typeindex>
#include <typeinfo>

namespace vix::game
{
  /**
   * @brief Runtime identifier for component types.
   *
   * ComponentTypeId is used by Registry to store and retrieve component
   * containers by C++ type.
   */
  using ComponentTypeId = std::type_index;

  /**
   * @brief Base type for optional polymorphic components.
   *
   * Components in vix/game do not have to inherit from this class. Plain structs
   * are supported and preferred for data-oriented code.
   *
   * This base exists only for users who want a common virtual interface.
   */
  class Component
  {
  public:
    /**
     * @brief Destroy the component.
     */
    virtual ~Component() = default;
  };

  /**
   * @brief Return a runtime component type id for T.
   *
   * @tparam T Component type.
   * @return Component type id.
   */
  template <typename T>
  [[nodiscard]] inline ComponentTypeId component_type_id() noexcept
  {
    return ComponentTypeId(typeid(T));
  }

  /**
   * @brief Return the implementation-defined component type name.
   *
   * This name is mainly useful for debugging and diagnostics.
   *
   * @tparam T Component type.
   * @return Static type name from typeid(T).name().
   */
  template <typename T>
  [[nodiscard]] inline std::string_view component_type_name() noexcept
  {
    return typeid(T).name();
  }

} // namespace vix::game

#endif // VIX_GAME_COMPONENT_HPP
