/**
 *
 *  @file AssetCache.cpp
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

#include <vix/game/AssetCache.hpp>

#include <utility>

#include <vix/game/GameError.hpp>

namespace vix::game
{
  GameBoolResult AssetCache::store(Asset asset)
  {
    if (!asset.valid())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "asset id is invalid");
    }

    const auto id = asset.id();
    const auto relative_path = asset.path().relative();

    if (relative_path.empty())
    {
      return make_game_error(
          GameErrorCode::AssetInvalidPath,
          "asset relative path cannot be empty");
    }

    if (contains(id))
    {
      return make_game_error(
          GameErrorCode::AssetAlreadyLoaded,
          "asset id is already cached");
    }

    if (contains(relative_path))
    {
      return make_game_error(
          GameErrorCode::AssetAlreadyLoaded,
          "asset path is already cached");
    }

    path_to_id_.emplace(relative_path, id);
    assets_.emplace(id, std::move(asset));

    return true;
  }

  GameBoolResult AssetCache::remove(AssetId id)
  {
    if (!is_valid_asset_id(id))
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "asset id is invalid");
    }

    const auto it = assets_.find(id);
    if (it == assets_.end())
    {
      return make_game_error(
          GameErrorCode::AssetNotFound,
          "asset not found");
    }

    path_to_id_.erase(it->second.path().relative());
    assets_.erase(it);

    return true;
  }

  GameBoolResult AssetCache::remove(const std::string &relative_path)
  {
    const auto id = id_for(relative_path);
    if (!is_valid_asset_id(id))
    {
      return make_game_error(
          GameErrorCode::AssetNotFound,
          "asset not found");
    }

    return remove(id);
  }

  bool AssetCache::contains(AssetId id) const
  {
    return assets_.find(id) != assets_.end();
  }

  bool AssetCache::contains(const std::string &relative_path) const
  {
    return path_to_id_.find(relative_path) != path_to_id_.end();
  }

  Asset *AssetCache::get(AssetId id)
  {
    const auto it = assets_.find(id);
    if (it == assets_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  const Asset *AssetCache::get(AssetId id) const
  {
    const auto it = assets_.find(id);
    if (it == assets_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  Asset *AssetCache::get(const std::string &relative_path)
  {
    const auto id = id_for(relative_path);
    if (!is_valid_asset_id(id))
    {
      return nullptr;
    }

    return get(id);
  }

  const Asset *AssetCache::get(const std::string &relative_path) const
  {
    const auto id = id_for(relative_path);
    if (!is_valid_asset_id(id))
    {
      return nullptr;
    }

    return get(id);
  }

  AssetId AssetCache::id_for(const std::string &relative_path) const
  {
    const auto it = path_to_id_.find(relative_path);
    if (it == path_to_id_.end())
    {
      return invalid_asset_id;
    }

    return it->second;
  }

  std::size_t AssetCache::size() const noexcept
  {
    return assets_.size();
  }

  bool AssetCache::empty() const noexcept
  {
    return assets_.empty();
  }

  std::vector<AssetId> AssetCache::ids() const
  {
    std::vector<AssetId> out;
    out.reserve(assets_.size());

    for (const auto &[id, asset] : assets_)
    {
      (void)asset;
      out.push_back(id);
    }

    return out;
  }

  std::vector<std::string> AssetCache::paths() const
  {
    std::vector<std::string> out;
    out.reserve(path_to_id_.size());

    for (const auto &[path, id] : path_to_id_)
    {
      (void)id;
      out.push_back(path);
    }

    return out;
  }

  AssetCacheStats AssetCache::stats() const
  {
    AssetCacheStats result;
    result.asset_count = assets_.size();

    for (const auto &[id, asset] : assets_)
    {
      (void)id;

      result.total_bytes += asset.size();

      if (asset.binary())
      {
        ++result.binary_asset_count;
      }
      else
      {
        ++result.text_asset_count;
      }
    }

    return result;
  }

  void AssetCache::clear()
  {
    assets_.clear();
    path_to_id_.clear();
  }

} // namespace vix::game
