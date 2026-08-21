/**
 *
 *  @file AsyncAssetLoader.hpp
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
#ifndef VIX_GAME_ASYNC_ASSET_LOADER_HPP
#define VIX_GAME_ASYNC_ASSET_LOADER_HPP

#include <functional>
#include <string>

#include <vix/game/AssetId.hpp>
#include <vix/game/AssetManager.hpp>
#include <vix/game/AssetType.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/JobHandle.hpp>
#include <vix/game/JobPriority.hpp>
#include <vix/game/JobSystem.hpp>
#include <vix/game/RuntimeDispatcher.hpp>

namespace vix::game
{
  /**
   * @brief Asynchronous asset loading helper.
   *
   * Workers only read files. Cache mutation and the completion callback are
   * posted to RuntimeDispatcher and run at the start of a later runtime frame.
   */
  class AsyncAssetLoader
  {
  public:
    /**
     * @brief Callback invoked when an async asset load completes.
     */
    using CompletionCallback = std::function<void(GameResult<AssetId>)>;

    /**
     * @brief Construct an async asset loader.
     *
     * @param assets Asset manager used to load and cache assets.
     * @param jobs Job system used to execute background work.
     */
    AsyncAssetLoader(
        AssetManager &assets,
        JobSystem &jobs,
        RuntimeDispatcher &dispatcher) noexcept;

    AsyncAssetLoader(const AsyncAssetLoader &) = delete;
    AsyncAssetLoader &operator=(const AsyncAssetLoader &) = delete;

    /**
     * @brief Destroy the async asset loader.
     */
    ~AsyncAssetLoader() = default;

    /**
     * @brief Load an asset asynchronously and infer its type from extension.
     *
     * @param relative_path Asset path relative to AssetManager::asset_root().
     * @param callback Completion callback.
     * @param priority Job priority.
     * @return Job handle, or a structured error.
     */
    [[nodiscard]] GameResult<JobHandle> load(
        std::string relative_path,
        CompletionCallback callback,
        JobPriority priority = JobPriority::Normal);

    /**
     * @brief Load an asset asynchronously with an explicit type.
     *
     * @param relative_path Asset path relative to AssetManager::asset_root().
     * @param type Explicit asset type.
     * @param callback Completion callback.
     * @param priority Job priority.
     * @return Job handle, or a structured error.
     */
    [[nodiscard]] GameResult<JobHandle> load_as(
        std::string relative_path,
        AssetType type,
        CompletionCallback callback,
        JobPriority priority = JobPriority::Normal);

    /**
     * @brief Reload an already loaded asset asynchronously.
     *
     * @param relative_path Asset path relative to AssetManager::asset_root().
     * @param callback Completion callback.
     * @param priority Job priority.
     * @return Job handle, or a structured error.
     */
    [[nodiscard]] GameResult<JobHandle> reload(
        std::string relative_path,
        CompletionCallback callback,
        JobPriority priority = JobPriority::Normal);

  private:
    [[nodiscard]] GameResult<JobHandle> submit_read(
        std::string relative_path,
        AssetType type,
        CompletionCallback callback,
        JobPriority priority,
        bool replace_existing);

    /**
     * @brief Asset manager used by this loader.
     */
    AssetManager *assets_{nullptr};

    /**
     * @brief Job system used by this loader.
     */
    JobSystem *jobs_{nullptr};

    /** Runtime queue used for cache mutation and user completion. */
    RuntimeDispatcher *dispatcher_{nullptr};
  };

} // namespace vix::game

#endif // VIX_GAME_ASYNC_ASSET_LOADER_HPP
