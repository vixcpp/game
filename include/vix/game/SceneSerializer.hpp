/**
 *
 *  @file SceneSerializer.hpp
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
#ifndef VIX_GAME_SCENE_SERIALIZER_HPP
#define VIX_GAME_SCENE_SERIALIZER_HPP

#include <functional>
#include <memory>
#include <string>

#include <vix/game/GameResult.hpp>
#include <vix/game/Scene.hpp>
#include <vix/game/SceneManager.hpp>
#include <vix/json/json.hpp>

namespace vix::game
{
  /**
   * @brief Serialize and restore scene manager state.
   *
   * SceneSerializer stores the scene list and active scene name.
   *
   * It does not serialize C++ object internals directly. Scene creation is
   * delegated to a user-provided factory because Scene is polymorphic.
   */
  class SceneSerializer
  {
  public:
    /**
     * @brief Factory used to create scene instances by name.
     */
    using SceneFactory = std::function<std::unique_ptr<Scene>(const std::string &name)>;

    /**
     * @brief Convert a scene manager state to JSON.
     *
     * @param manager Scene manager to serialize.
     * @return JSON object.
     */
    [[nodiscard]] static vix::json::Json to_json(
        const SceneManager &manager);

    /**
     * @brief Restore a scene manager from JSON.
     *
     * Existing scenes are cleared before loading new scenes.
     *
     * @param manager Destination scene manager.
     * @param json Source JSON object.
     * @param factory Factory used to create scene instances.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] static GameBoolResult from_json(
        SceneManager &manager,
        const vix::json::Json &json,
        SceneFactory factory);

    /**
     * @brief Save a scene manager state to a file.
     *
     * @param manager Scene manager to serialize.
     * @param path Destination file path.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] static GameBoolResult save_file(
        const SceneManager &manager,
        const std::string &path);

    /**
     * @brief Load a scene manager state from a file.
     *
     * Existing scenes are cleared before loading new scenes.
     *
     * @param manager Destination scene manager.
     * @param path Source file path.
     * @param factory Factory used to create scene instances.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] static GameBoolResult load_file(
        SceneManager &manager,
        const std::string &path,
        SceneFactory factory);
  };

} // namespace vix::game

#endif // VIX_GAME_SCENE_SERIALIZER_HPP
