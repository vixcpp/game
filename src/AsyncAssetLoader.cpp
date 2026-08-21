/**
 *
 *  @file AsyncAssetLoader.cpp
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

#include <vix/game/AsyncAssetLoader.hpp>

#include <exception>
#include <utility>

#include <vix/game/GameError.hpp>
#include <vix/game/AssetPath.hpp>

namespace vix::game
{
  AsyncAssetLoader::AsyncAssetLoader(
      AssetManager &assets,
      JobSystem &jobs,
      RuntimeDispatcher &dispatcher) noexcept
      : assets_(&assets),
        jobs_(&jobs),
        dispatcher_(&dispatcher)
  {
  }

  GameResult<JobHandle> AsyncAssetLoader::load(
      std::string relative_path,
      CompletionCallback callback,
      JobPriority priority)
  {
    if (!callback)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "async asset completion callback cannot be empty");
    }

    if (relative_path.empty())
    {
      return make_game_error(
          GameErrorCode::AssetInvalidPath,
          "asset path cannot be empty");
    }

    auto path = AssetPath::make(assets_->asset_root(), relative_path);
    if (!path)
    {
      return path.error();
    }
    auto extension = path.value().extension();
    if (!extension)
    {
      return extension.error();
    }
    return submit_read(std::move(relative_path), asset_type_from_extension(extension.value()),
                       std::move(callback), priority, false);
  }

  GameResult<JobHandle> AsyncAssetLoader::load_as(
      std::string relative_path,
      AssetType type,
      CompletionCallback callback,
      JobPriority priority)
  {
    if (!callback)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "async asset completion callback cannot be empty");
    }

    if (relative_path.empty())
    {
      return make_game_error(
          GameErrorCode::AssetInvalidPath,
          "asset path cannot be empty");
    }

    return submit_read(std::move(relative_path), type, std::move(callback), priority, false);
  }

  GameResult<JobHandle> AsyncAssetLoader::reload(
      std::string relative_path,
      CompletionCallback callback,
      JobPriority priority)
  {
    if (!callback)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "async asset completion callback cannot be empty");
    }

    if (relative_path.empty())
    {
      return make_game_error(
          GameErrorCode::AssetInvalidPath,
          "asset path cannot be empty");
    }

    auto path = AssetPath::make(assets_->asset_root(), relative_path);
    if (!path)
    {
      return path.error();
    }
    auto extension = path.value().extension();
    if (!extension)
    {
      return extension.error();
    }
    return submit_read(std::move(relative_path), asset_type_from_extension(extension.value()),
                       std::move(callback), priority, true);
  }

  GameResult<JobHandle> AsyncAssetLoader::submit_read(
      std::string relative_path,
      AssetType type,
      CompletionCallback callback,
      JobPriority priority,
      bool replace_existing)
  {
    // Capture only immutable input in the worker. In particular, it must not
    // access AssetManager, EventBus, SceneManager, or GameContext services.
    const std::string asset_root = assets_->asset_root();
    return jobs_->submit(
        [assets = assets_, dispatcher = dispatcher_, asset_root,
         path = std::move(relative_path), type, callback = std::move(callback),
         replace_existing]() mutable
        {
          GameResult<Asset> asset = make_game_error(
              GameErrorCode::AssetLoadFailed, "async asset loading failed");
          try
          {
            asset = AssetManager::read_from_disk(asset_root, path, type);
          }
          catch (const std::exception &e)
          {
            asset = make_game_error(GameErrorCode::AssetLoadFailed, e.what());
          }
          catch (...)
          {
          }
          (void)dispatcher->post(
              [assets, path = std::move(path), callback = std::move(callback),
               asset = std::move(asset), replace_existing]() mutable
              {
                GameResult<AssetId> result = asset ? [&]() -> GameResult<AssetId>
                {
                  if (replace_existing && assets->contains(path))
                  {
                    auto unloaded = assets->unload(path);
                    if (!unloaded)
                    {
                      return unloaded.error();
                    }
                  }
                  return assets->commit(std::move(asset.value()));
                }() : GameResult<AssetId>(asset.error());
                callback(std::move(result));
              });
        },
        priority);
  }

} // namespace vix::game
