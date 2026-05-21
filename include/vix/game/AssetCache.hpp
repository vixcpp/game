/**
 *
 *  @file AssetCache.hpp
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
#ifndef VIX_GAME_ASSET_CACHE_HPP
#define VIX_GAME_ASSET_CACHE_HPP

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include <vix/game/Asset.hpp>
#include <vix/game/AssetId.hpp>
#include <vix/game/GameResult.hpp>

namespace vix::game
{
  /**
   * @brief Runtime statistics for an asset cache.
   */
  struct AssetCacheStats
  {
    /**
     * @brief Number of cached assets.
     */
    std::size_t asset_count{0};

    /**
     * @brief Number of cached text assets.
     */
    std::size_t text_asset_count{0};

    /**
     * @brief Number of cached binary assets.
     */
    std::size_t binary_asset_count{0};

    /**
     * @brief Total cached bytes.
     */
    std::size_t total_bytes{0};
  };

  /**
   * @brief In-memory cache for loaded assets.
   *
   * AssetCache owns loaded Asset objects and provides lookup by:
   * - asset id
   * - relative asset path
   *
   * It is intentionally independent from filesystem loading. AssetManager loads
   * assets, while AssetCache stores and indexes them.
   */
  class AssetCache
  {
  public:
    /**
     * @brief Construct an empty asset cache.
     */
    AssetCache() = default;

    AssetCache(const AssetCache &) = delete;
    AssetCache &operator=(const AssetCache &) = delete;

    /**
     * @brief Destroy the asset cache.
     */
    ~AssetCache() = default;

    /**
     * @brief Store an asset in the cache.
     *
     * @param asset Loaded asset.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult store(Asset asset);

    /**
     * @brief Remove an asset by id.
     *
     * @param id Asset id.
     * @return true if the asset was removed.
     */
    [[nodiscard]] GameBoolResult remove(AssetId id);

    /**
     * @brief Remove an asset by relative path.
     *
     * @param relative_path Relative asset path.
     * @return true if the asset was removed.
     */
    [[nodiscard]] GameBoolResult remove(const std::string &relative_path);

    /**
     * @brief Return true if an asset id exists.
     *
     * @param id Asset id.
     * @return true if cached.
     */
    [[nodiscard]] bool contains(AssetId id) const;

    /**
     * @brief Return true if a relative path exists.
     *
     * @param relative_path Relative asset path.
     * @return true if cached.
     */
    [[nodiscard]] bool contains(const std::string &relative_path) const;

    /**
     * @brief Return an asset by id.
     *
     * @param id Asset id.
     * @return Asset pointer, or nullptr if missing.
     */
    [[nodiscard]] Asset *get(AssetId id);

    /**
     * @brief Return an asset by id.
     *
     * @param id Asset id.
     * @return Asset pointer, or nullptr if missing.
     */
    [[nodiscard]] const Asset *get(AssetId id) const;

    /**
     * @brief Return an asset by relative path.
     *
     * @param relative_path Relative asset path.
     * @return Asset pointer, or nullptr if missing.
     */
    [[nodiscard]] Asset *get(const std::string &relative_path);

    /**
     * @brief Return an asset by relative path.
     *
     * @param relative_path Relative asset path.
     * @return Asset pointer, or nullptr if missing.
     */
    [[nodiscard]] const Asset *get(const std::string &relative_path) const;

    /**
     * @brief Return an asset id for a relative path.
     *
     * @param relative_path Relative asset path.
     * @return Asset id, or invalid_asset_id if missing.
     */
    [[nodiscard]] AssetId id_for(const std::string &relative_path) const;

    /**
     * @brief Return cached asset count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Return true if the cache is empty.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Return all cached asset ids.
     */
    [[nodiscard]] std::vector<AssetId> ids() const;

    /**
     * @brief Return all cached relative paths.
     */
    [[nodiscard]] std::vector<std::string> paths() const;

    /**
     * @brief Return cache statistics.
     */
    [[nodiscard]] AssetCacheStats stats() const;

    /**
     * @brief Remove all cached assets.
     */
    void clear();

  private:
    /**
     * @brief Cached assets by id.
     */
    std::unordered_map<AssetId, Asset> assets_{};

    /**
     * @brief Asset id lookup by relative path.
     */
    std::unordered_map<std::string, AssetId> path_to_id_{};
  };

} // namespace vix::game

#endif // VIX_GAME_ASSET_CACHE_HPP
