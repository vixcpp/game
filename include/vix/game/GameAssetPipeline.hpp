/**
 *
 *  @file GameAssetPipeline.hpp
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
#ifndef VIX_GAME_GAME_ASSET_PIPELINE_HPP
#define VIX_GAME_GAME_ASSET_PIPELINE_HPP

#include <filesystem>
#include <vector>

#include <vix/game/GameExportAsset.hpp>
#include <vix/game/GameResult.hpp>

namespace vix::game
{
  /**
   * @brief Scans and classifies assets during game export.
   *
   * GameAssetPipeline is intentionally simple in V5. It does not transform or
   * compress assets yet. It only discovers assets, counts them, classifies them,
   * and ignores temporary/cache/build files.
   */
  class GameAssetPipeline
  {
  public:
    /**
     * @brief Scan an asset directory.
     *
     * @param project_root Project root directory.
     * @param asset_root Asset directory relative to project_root.
     * @return Export asset list, or a structured error.
     */
    [[nodiscard]] GameResult<std::vector<GameExportAsset>> scan_assets(
        const std::filesystem::path &project_root,
        const std::filesystem::path &asset_root) const;

    /**
     * @brief Detect the export asset type from a file path.
     */
    [[nodiscard]] static GameExportAssetType detect_type(
        const std::filesystem::path &path);

    /**
     * @brief Return true if a file or directory should be ignored.
     */
    [[nodiscard]] static bool should_ignore(
        const std::filesystem::path &path);
  };

} // namespace vix::game

#endif // VIX_GAME_GAME_ASSET_PIPELINE_HPP
