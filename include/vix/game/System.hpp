/**
 *
 *  @file System.hpp
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
#ifndef VIX_GAME_SYSTEM_HPP
#define VIX_GAME_SYSTEM_HPP

#include <string>
#include <utility>

#include <vix/game/Frame.hpp>
#include <vix/game/GameResult.hpp>

namespace vix::game
{
  class Registry;

  /**
   * @brief Base class for game systems.
   *
   * System represents logic that operates over entities and components stored
   * inside a Registry.
   *
   * Examples:
   * - movement system
   * - animation system
   * - collision system
   * - gameplay system
   *
   * Systems are optional in V1. Users can also update entities directly.
   */
  class System
  {
  public:
    /**
     * @brief Construct an unnamed system.
     */
    System() = default;

    /**
     * @brief Construct a system with a name.
     *
     * @param name System name.
     */
    explicit System(std::string name)
        : name_(std::move(name))
    {
    }

    System(const System &) = delete;
    System &operator=(const System &) = delete;

    /**
     * @brief Destroy the system.
     */
    virtual ~System() = default;

    /**
     * @brief Called when the system is attached to a registry.
     *
     * @param registry Registry that owns the entities/components.
     */
    virtual void on_attach(Registry &registry)
    {
      registry_ = &registry;
    }

    /**
     * @brief Called when the system is detached from its registry.
     */
    virtual void on_detach()
    {
      registry_ = nullptr;
    }

    /**
     * @brief Called when the system starts.
     *
     * @return true on success, or a structured error.
     */
    [[nodiscard]] virtual GameBoolResult on_start()
    {
      started_ = true;
      return true;
    }

    /**
     * @brief Called once per variable frame update.
     *
     * @param frame Current frame.
     */
    virtual void on_update(const Frame &frame)
    {
      (void)frame;
    }

    /**
     * @brief Called for each fixed simulation step.
     *
     * @param frame Current frame.
     */
    virtual void on_fixed_update(const Frame &frame)
    {
      (void)frame;
    }

    /**
     * @brief Called when the system stops.
     */
    virtual void on_stop()
    {
      started_ = false;
    }

    /**
     * @brief Return the system name.
     */
    [[nodiscard]] const std::string &name() const noexcept
    {
      return name_;
    }

    /**
     * @brief Set the system name.
     *
     * @param value New system name.
     * @return Reference to this system.
     */
    System &set_name(std::string value)
    {
      name_ = std::move(value);
      return *this;
    }

    /**
     * @brief Return true if the system is attached to a registry.
     */
    [[nodiscard]] bool attached() const noexcept
    {
      return registry_ != nullptr;
    }

    /**
     * @brief Return true if the system has started.
     */
    [[nodiscard]] bool started() const noexcept
    {
      return started_;
    }

  protected:
    /**
     * @brief Return the attached registry.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] Registry &registry()
    {
      return *registry_;
    }

    /**
     * @brief Return the attached registry.
     *
     * @warning Must only be called when attached() is true.
     */
    [[nodiscard]] const Registry &registry() const
    {
      return *registry_;
    }

  private:
    /**
     * @brief Optional system name.
     */
    std::string name_{};

    /**
     * @brief Attached registry.
     */
    Registry *registry_{nullptr};

    /**
     * @brief Whether the system has started.
     */
    bool started_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_SYSTEM_HPP
