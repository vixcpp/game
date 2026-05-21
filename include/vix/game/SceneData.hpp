/**
 *
 *  @file SceneData.hpp
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
#ifndef VIX_GAME_SCENE_DATA_HPP
#define VIX_GAME_SCENE_DATA_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <vix/game/GameResult.hpp>
#include <vix/game/SceneId.hpp>
#include <vix/json/json.hpp>

namespace vix::game
{
  /**
   * @brief Serializable scene entry.
   *
   * SceneData is a plain data representation of one scene.
   *
   * It is used by SceneSerializer to save and restore scene manager state
   * without depending on the concrete C++ Scene implementation.
   */
  struct SceneData
  {
    /**
     * @brief Scene id at serialization time.
     */
    SceneId id{invalid_scene_id};

    /**
     * @brief Scene name.
     */
    std::string name{};

    /**
     * @brief Whether the scene was loaded.
     */
    bool loaded{false};

    /**
     * @brief Whether the scene was active.
     */
    bool active{false};

    /**
     * @brief Convert this scene data to JSON.
     *
     * @return JSON object.
     */
    [[nodiscard]] vix::json::Json to_json() const;

    /**
     * @brief Build scene data from JSON.
     *
     * @param json Source JSON object.
     * @return SceneData or a structured error.
     */
    [[nodiscard]] static GameResult<SceneData> from_json(
        const vix::json::Json &json);

    /**
     * @brief Validate this scene data.
     *
     * @return true if valid, otherwise a structured error.
     */
    [[nodiscard]] GameBoolResult validate() const;
  };

  /**
   * @brief Serializable scene manager state.
   *
   * SceneFileData stores the data needed by SceneSerializer:
   * - serialization format version
   * - active scene name
   * - registered scenes
   */
  struct SceneFileData
  {
    /**
     * @brief Scene file format version.
     */
    std::uint32_t version{1};

    /**
     * @brief Active scene name.
     */
    std::string active_scene{};

    /**
     * @brief Serialized scenes.
     */
    std::vector<SceneData> scenes{};

    /**
     * @brief Convert this file data to JSON.
     *
     * @return JSON object.
     */
    [[nodiscard]] vix::json::Json to_json() const;

    /**
     * @brief Build scene file data from JSON.
     *
     * @param json Source JSON object.
     * @return SceneFileData or a structured error.
     */
    [[nodiscard]] static GameResult<SceneFileData> from_json(
        const vix::json::Json &json);

    /**
     * @brief Validate this scene file data.
     *
     * @return true if valid, otherwise a structured error.
     */
    [[nodiscard]] GameBoolResult validate() const;
  };

} // namespace vix::game

#endif // VIX_GAME_SCENE_DATA_HPP
