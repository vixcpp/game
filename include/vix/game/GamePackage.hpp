/**
 *
 *  @file GamePackage.hpp
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
#ifndef VIX_GAME_GAME_PACKAGE_HPP
#define VIX_GAME_GAME_PACKAGE_HPP

#include <string>
#include <vector>

#include <vix/game/GameResult.hpp>
#include <vix/json/json.hpp>

namespace vix::game
{
  /**
   * @brief Game package metadata.
   *
   * GamePackage describes a game project in a small, serializable format.
   *
   * It is the foundation for future workflows such as:
   * - vix new game
   * - vix game package
   * - vix game export
   * - editor project loading
   */
  class GamePackage
  {
  public:
    /**
     * @brief Create a default game package.
     */
    GamePackage() = default;

    /**
     * @brief Return a default package.
     */
    [[nodiscard]] static GamePackage defaults();

    /**
     * @brief Validate the package.
     *
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult validate() const;

    /**
     * @brief Convert this package to JSON.
     */
    [[nodiscard]] vix::json::Json to_json() const;

    /**
     * @brief Build a package from JSON.
     *
     * @param json Source JSON object.
     * @return Game package, or a structured error.
     */
    [[nodiscard]] static GameResult<GamePackage> from_json(
        const vix::json::Json &json);

    /**
     * @brief Save this package to a file.
     *
     * @param path Destination file path.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult save_file(const std::string &path) const;

    /**
     * @brief Load a package from a file.
     *
     * @param path Source file path.
     * @return Game package, or a structured error.
     */
    [[nodiscard]] static GameResult<GamePackage> load_file(
        const std::string &path);

  public:
    std::string name{"vix-game"};
    std::string version{"0.1.0"};
    std::string author{};
    std::string entry_scene{"main"};
    std::string asset_root{"assets"};
    std::string output_dir{"dist"};

    std::vector<std::string> scenes{};
    std::vector<std::string> assets{};
  };

} // namespace vix::game

#endif // VIX_GAME_GAME_PACKAGE_HPP
