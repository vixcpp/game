/**
 *
 *  @file AssetManager.hpp
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
#ifndef VIX_GAME_ASSET_MANAGER_HPP
#define VIX_GAME_ASSET_MANAGER_HPP

#include <cstddef>
#include <string>
#include <vector>

#include <vix/game/Asset.hpp>
#include <vix/game/AssetCache.hpp>
#include <vix/game/AssetId.hpp>
#include <vix/game/AssetType.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/EventType.hpp>

namespace vix::game
{
  class EventBus;

  /**
   * @brief Loads, stores, retrieves, and unloads game assets.
   *
   * AssetManager is responsible for filesystem loading and event dispatch.
   * AssetCache owns the loaded assets and provides lookup/indexing.
   */
  class AssetManager
  {
  public:
    /**
     * @brief Construct an asset manager with default asset root.
     */
    AssetManager() = default;

    /**
     * @brief Construct an asset manager with explicit asset root.
     *
     * @param asset_root Root directory used to resolve assets.
     */
    explicit AssetManager(std::string asset_root);

    AssetManager(const AssetManager &) = delete;
    AssetManager &operator=(const AssetManager &) = delete;

    /**
     * @brief Destroy the asset manager.
     */
    ~AssetManager() = default;

    /**
     * @brief Set the event bus used for asset events.
     *
     * @param bus Event bus pointer. May be nullptr.
     */
    void set_event_bus(EventBus *bus) noexcept;

    /**
     * @brief Return the current asset root.
     */
    [[nodiscard]] const std::string &asset_root() const noexcept;

    /**
     * @brief Set the asset root.
     *
     * @param value New asset root.
     * @return Reference to this manager.
     */
    AssetManager &set_asset_root(std::string value);

    /**
     * @brief Load an asset by relative path.
     *
     * The asset type is inferred from the file extension.
     *
     * @param relative_path Path relative to asset_root().
     * @return Loaded asset id, or a structured error.
     */
    [[nodiscard]] GameResult<AssetId> load(const std::string &relative_path);

    /**
     * @brief Load an asset if missing, otherwise return its existing id.
     *
     * @param relative_path Path relative to asset_root().
     * @return Existing or loaded asset id, or a structured error.
     */
    [[nodiscard]] GameResult<AssetId> load_or_get(
        const std::string &relative_path);

    /**
     * @brief Load an asset by relative path and explicit type.
     *
     * @param relative_path Path relative to asset_root().
     * @param type Asset type.
     * @return Loaded asset id, or a structured error.
     */
    [[nodiscard]] GameResult<AssetId> load_as(
        const std::string &relative_path,
        AssetType type);

    /**
     * @brief Reload an already loaded asset.
     *
     * @param relative_path Relative asset path.
     * @return Reloaded asset id, or a structured error.
     */
    [[nodiscard]] GameResult<AssetId> reload(const std::string &relative_path);

    /**
     * @brief Unload an asset by id.
     *
     * @param id Asset id.
     * @return true if the asset was unloaded.
     */
    [[nodiscard]] GameBoolResult unload(AssetId id);

    /**
     * @brief Unload an asset by relative path.
     *
     * @param relative_path Relative asset path.
     * @return true if the asset was unloaded.
     */
    [[nodiscard]] GameBoolResult unload(const std::string &relative_path);

    /**
     * @brief Return true if an asset id exists.
     */
    [[nodiscard]] bool contains(AssetId id) const;

    /**
     * @brief Return true if a relative asset path exists.
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
     * @brief Return loaded asset count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Return true if no asset is loaded.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Return all loaded asset ids.
     */
    [[nodiscard]] std::vector<AssetId> ids() const;

    /**
     * @brief Return all loaded asset paths.
     */
    [[nodiscard]] std::vector<std::string> paths() const;

    /**
     * @brief Return cache statistics.
     */
    [[nodiscard]] AssetCacheStats stats() const;

    /**
     * @brief Return the underlying asset cache.
     */
    [[nodiscard]] AssetCache &cache() noexcept;

    /**
     * @brief Return the underlying asset cache.
     */
    [[nodiscard]] const AssetCache &cache() const noexcept;

    /**
     * @brief Remove all loaded assets.
     */
    void clear();

  private:
    /**
     * @brief Load an asset from disk and build an Asset object.
     *
     * @param relative_path Relative asset path.
     * @param type Asset type.
     * @param id Asset id to assign.
     * @return Loaded asset or a structured error.
     */
    [[nodiscard]] GameResult<Asset> load_asset_from_disk(
        const std::string &relative_path,
        AssetType type,
        AssetId id);

    /**
     * @brief Dispatch an asset lifecycle event if an event bus is available.
     *
     * @param type Event type.
     * @param relative_path Asset relative path.
     */
    void dispatch_asset_event(
        EventType type,
        const std::string &relative_path);

    /**
     * @brief Return the next asset id.
     */
    [[nodiscard]] AssetId next_id() noexcept;

  private:
    /**
     * @brief Root directory used to resolve assets.
     */
    std::string asset_root_{"assets"};

    /**
     * @brief In-memory asset cache.
     */
    AssetCache cache_{};

    /**
     * @brief Event bus used for asset events.
     */
    EventBus *events_{nullptr};

    /**
     * @brief Next asset id.
     */
    AssetId next_id_{1};
  };

} // namespace vix::game

#endif // VIX_GAME_ASSET_MANAGER_HPP
