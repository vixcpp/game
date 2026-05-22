/**
 *
 *  @file GameExporter.hpp
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
#ifndef VIX_GAME_GAME_EXPORTER_HPP
#define VIX_GAME_GAME_EXPORTER_HPP

#include <cstdint>
#include <filesystem>
#include <string>

#include <vix/game/GameExportConfig.hpp>
#include <vix/game/GameResult.hpp>

namespace vix::game
{
  /**
   * @brief Result produced after exporting a game project.
   *
   * GameExportResult describes the final export output and the number of
   * resources copied or generated during the export process.
   */
  class GameExportResult
  {
  public:
    /**
     * @brief Final exported directory.
     */
    std::filesystem::path output_path{};

    /**
     * @brief Exported game name.
     *
     * This value may come from GameExportConfig or from game.package.json when
     * the package file exists.
     */
    std::string name{"game"};

    /**
     * @brief Exported game version.
     *
     * This value is read from game.package.json when available.
     */
    std::string version{"0.1.0"};

    /**
     * @brief Asset root used during export.
     *
     * This value may come from GameExportConfig or from game.package.json.
     */
    std::string asset_root{"assets"};

    /**
     * @brief Number of copied or generated files.
     *
     * This includes files copied from the project and generated files such as
     * export.json.
     */
    std::uint64_t copied_files{0};

    /**
     * @brief Number of copied directories.
     */
    std::uint64_t copied_directories{0};
  };

  /**
   * @brief Export a vix/game project into a distributable directory.
   *
   * GameExporter is intentionally dependency-free.
   *
   * It prepares the foundation for future commands such as:
   * - vix game export
   * - vix game package
   * - editor export workflow
   * - asset packaging workflow
   */
  class GameExporter
  {
  public:
    /**
     * @brief Export a game project.
     *
     * @param config Export configuration.
     * @return Export result or structured game error.
     */
    [[nodiscard]] GameResult<GameExportResult> export_project(
        const GameExportConfig &config) const;

  private:
    /**
     * @brief Copy one file if it exists.
     */
    [[nodiscard]] static GameResult<std::uint64_t> copy_file_if_exists(
        const std::filesystem::path &from,
        const std::filesystem::path &to);

    /**
     * @brief Copy a directory recursively if it exists.
     */
    [[nodiscard]] static GameResult<GameExportResult> copy_directory_if_exists(
        const std::filesystem::path &from,
        const std::filesystem::path &to);
  };

} // namespace vix::game

#endif // VIX_GAME_GAME_EXPORTER_HPP
