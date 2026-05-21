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

namespace vix::game
{
  AsyncAssetLoader::AsyncAssetLoader(
      AssetManager &assets,
      JobSystem &jobs) noexcept
      : assets_(&assets),
        jobs_(&jobs)
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

    return jobs_->submit(
        [assets = assets_,
         path = std::move(relative_path),
         callback = std::move(callback)]() mutable
        {
          try
          {
            callback(assets->load(path));
          }
          catch (const std::exception &e)
          {
            callback(make_game_error(
                GameErrorCode::AssetLoadFailed,
                e.what()));
          }
          catch (...)
          {
            callback(make_game_error(
                GameErrorCode::AssetLoadFailed,
                "async asset loading failed"));
          }
        },
        priority);
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

    return jobs_->submit(
        [assets = assets_,
         path = std::move(relative_path),
         type,
         callback = std::move(callback)]() mutable
        {
          try
          {
            callback(assets->load_as(path, type));
          }
          catch (const std::exception &e)
          {
            callback(make_game_error(
                GameErrorCode::AssetLoadFailed,
                e.what()));
          }
          catch (...)
          {
            callback(make_game_error(
                GameErrorCode::AssetLoadFailed,
                "async asset loading failed"));
          }
        },
        priority);
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

    return jobs_->submit(
        [assets = assets_,
         path = std::move(relative_path),
         callback = std::move(callback)]() mutable
        {
          try
          {
            callback(assets->reload(path));
          }
          catch (const std::exception &e)
          {
            callback(make_game_error(
                GameErrorCode::AssetLoadFailed,
                e.what()));
          }
          catch (...)
          {
            callback(make_game_error(
                GameErrorCode::AssetLoadFailed,
                "async asset reload failed"));
          }
        },
        priority);
  }

} // namespace vix::game
