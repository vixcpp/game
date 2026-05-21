/**
 *
 *  @file AppConfig.hpp
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
#ifndef VIX_GAME_APP_CONFIG_HPP
#define VIX_GAME_APP_CONFIG_HPP

#include <cstdint>
#include <string>

#include <vix/game/GameResult.hpp>
#include <vix/game/TimeStep.hpp>
#include <vix/json/json.hpp>

namespace vix::game
{
  /**
   * @brief Configuration used to initialize a game application.
   *
   * AppConfig stores the minimal V1 configuration needed by vix/game:
   * - app name and title
   * - asset root
   * - start scene
   * - timing rules
   * - basic headless mode
   *
   * It is intentionally independent from graphics/windowing APIs.
   */
  struct AppConfig
  {
    /**
     * @brief Internal application name.
     */
    std::string name{"vix_game"};

    /**
     * @brief Human-readable title.
     */
    std::string title{"Vix Game"};

    /**
     * @brief Root directory used to resolve assets.
     */
    std::string asset_root{"assets"};

    /**
     * @brief First scene to load when the app starts.
     */
    std::string start_scene{"main"};

    /**
     * @brief Timing configuration for the main loop.
     */
    TimeStep time_step{};

    /**
     * @brief Whether the app runs without graphics/windowing.
     */
    bool headless{true};

    /**
     * @brief Whether startup logs should be printed.
     */
    bool log_startup{true};

    /**
     * @brief Whether shutdown logs should be printed.
     */
    bool log_shutdown{true};

    /**
     * @brief Create default app config.
     */
    [[nodiscard]] static AppConfig defaults();

    /**
     * @brief Load app config from a JSON file.
     *
     * @param path Path to the configuration file.
     * @return Parsed config or a structured error.
     */
    [[nodiscard]] static GameResult<AppConfig> load_file(const std::string &path);

    /**
     * @brief Build app config from a JSON object.
     *
     * @param json JSON object.
     * @return Parsed config or a structured error.
     */
    [[nodiscard]] static GameResult<AppConfig> from_json(const vix::json::Json &json);

    /**
     * @brief Convert the config to JSON.
     *
     * @return JSON representation.
     */
    [[nodiscard]] vix::json::Json to_json() const;

    /**
     * @brief Validate the config.
     *
     * @return true if valid, otherwise a structured error.
     */
    [[nodiscard]] GameBoolResult validate() const;

    /**
     * @brief Set the target FPS in the embedded timestep.
     *
     * @param fps Target frames per second.
     * @return Reference to this config.
     */
    AppConfig &set_target_fps(std::uint32_t fps) noexcept;

    /**
     * @brief Set the app title.
     *
     * @param value New title.
     * @return Reference to this config.
     */
    AppConfig &set_title(std::string value);

    /**
     * @brief Set the asset root.
     *
     * @param value Asset root path.
     * @return Reference to this config.
     */
    AppConfig &set_asset_root(std::string value);

    /**
     * @brief Set the start scene.
     *
     * @param value Start scene name.
     * @return Reference to this config.
     */
    AppConfig &set_start_scene(std::string value);
  };

} // namespace vix::game

#endif // VIX_GAME_APP_CONFIG_HPP
