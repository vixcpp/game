/**
 *
 *  @file RuntimeDiagnostics.hpp
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
#ifndef VIX_GAME_RUNTIME_DIAGNOSTICS_HPP
#define VIX_GAME_RUNTIME_DIAGNOSTICS_HPP

#include <cstddef>
#include <cstdint>
#include <string>

namespace vix::game
{
  /**
   * @brief Lightweight runtime diagnostics snapshot.
   *
   * RuntimeDiagnostics exposes frame, window, renderer, asset, scene, and ECS
   * state for debugging, editor tools, and runtime inspection.
   */
  struct RuntimeDiagnostics
  {
    /**
     * @brief Last observed frame index.
     */
    std::uint64_t frame_index{0};

    /**
     * @brief Last observed frame delta in milliseconds.
     */
    std::int64_t delta_ms{0};

    /**
     * @brief Window backend name.
     */
    std::string window_backend{"none"};

    /**
     * @brief Renderer backend name.
     */
    std::string renderer_backend{"none"};

    /**
     * @brief Current window width.
     */
    std::uint32_t window_width{0};

    /**
     * @brief Current window height.
     */
    std::uint32_t window_height{0};

    /**
     * @brief Number of submitted 2D sprite commands.
     */
    std::size_t sprite_command_count{0};

    /**
     * @brief Number of loaded assets.
     */
    std::size_t loaded_asset_count{0};

    /**
     * @brief Number of registered scenes.
     */
    std::size_t scene_count{0};

    /**
     * @brief Active scene name.
     */
    std::string active_scene{};

    /**
     * @brief Number of entities in the inspected registry.
     */
    std::size_t entity_count{0};

    /**
     * @brief Number of systems in the inspected registry.
     */
    std::size_t system_count{0};

    /**
     * @brief Return true if a window backend is installed.
     */
    [[nodiscard]] bool has_window_backend() const noexcept
    {
      return window_backend != "none";
    }

    /**
     * @brief Return true if a renderer backend is installed.
     */
    [[nodiscard]] bool has_renderer_backend() const noexcept
    {
      return renderer_backend != "none";
    }

    /**
     * @brief Return true if an active scene exists.
     */
    [[nodiscard]] bool has_active_scene() const noexcept
    {
      return !active_scene.empty();
    }
  };

} // namespace vix::game

#endif // VIX_GAME_RUNTIME_DIAGNOSTICS_HPP
