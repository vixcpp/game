/**
 *
 *  @file GameExportConfig.hpp
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
#ifndef VIX_GAME_GAME_EXPORT_CONFIG_HPP
#define VIX_GAME_GAME_EXPORT_CONFIG_HPP

#include <filesystem>
#include <string>

#include <vix/game/GameResult.hpp>

namespace vix::game
{
  /**
   * @brief Configuration used to export a game project.
   *
   * GameExportConfig describes where the project is located, where the
   * exported files should be written, and which optional resources should be
   * included in the exported output.
   */
  class GameExportConfig
  {
  public:
    /**
     * @brief Project root directory.
     */
    std::filesystem::path project_root{"."};

    /**
     * @brief Output directory used for the exported game.
     */
    std::filesystem::path output_directory{"dist"};

    /**
     * @brief Asset directory relative to the project root.
     */
    std::filesystem::path asset_directory{"assets"};

    /**
     * @brief Game package file relative to the project root.
     */
    std::filesystem::path package_file{"game.package.json"};

    /**
     * @brief Optional README file relative to the project root.
     */
    std::filesystem::path readme_file{"README.md"};

    /**
     * @brief Export name.
     *
     * When empty, the exporter uses the package name if available or "game".
     */
    std::string name{"game"};

    /**
     * @brief Whether assets should be copied.
     */
    bool copy_assets{true};

    /**
     * @brief Whether game.package.json should be copied.
     */
    bool copy_package_file{true};

    /**
     * @brief Whether README.md should be copied when it exists.
     */
    bool copy_readme{true};

    /**
     * @brief Whether an existing export directory can be overwritten.
     */
    bool overwrite{true};

    /**
     * @brief Validate the export configuration.
     */
    [[nodiscard]] GameBoolResult validate() const;
  };

} // namespace vix::game

#endif // VIX_GAME_GAME_EXPORT_CONFIG_HPP
