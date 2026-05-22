/**
 *
 *  @file AssetManager.cpp
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

#include <vix/game/AssetManager.hpp>

#include <utility>

#include <vix/fs/Exists.hpp>
#include <vix/fs/IsFile.hpp>
#include <vix/fs/ReadFile.hpp>
#include <vix/fs/ReadText.hpp>
#include <vix/game/AssetPath.hpp>
#include <vix/game/Event.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/EventType.hpp>
#include <vix/game/GameError.hpp>

namespace vix::game
{
  AssetManager::AssetManager(std::string asset_root)
      : asset_root_(std::move(asset_root)),
        cache_(),
        events_(nullptr),
        next_id_(1)
  {
  }

  void AssetManager::set_event_bus(EventBus *bus) noexcept
  {
    events_ = bus;
  }

  const std::string &AssetManager::asset_root() const noexcept
  {
    return asset_root_;
  }

  AssetManager &AssetManager::set_asset_root(std::string value)
  {
    asset_root_ = std::move(value);
    return *this;
  }

  GameResult<AssetId> AssetManager::load(const std::string &relative_path)
  {
    auto path_result = AssetPath::make(asset_root_, relative_path);
    if (!path_result)
    {
      return path_result.error();
    }

    auto extension = path_result.value().extension();
    if (!extension)
    {
      return extension.error();
    }

    const AssetType type = asset_type_from_extension(extension.value());
    return load_as(relative_path, type);
  }

  GameResult<AssetId> AssetManager::load_or_get(
      const std::string &relative_path)
  {
    if (relative_path.empty())
    {
      return make_game_error(
          GameErrorCode::AssetInvalidPath,
          "asset path cannot be empty");
    }

    const AssetId existing = cache_.id_for(relative_path);
    if (is_valid_asset_id(existing))
    {
      return existing;
    }

    return load(relative_path);
  }

  GameResult<AssetId> AssetManager::load_as(
      const std::string &relative_path,
      AssetType type)
  {
    if (relative_path.empty())
    {
      return make_game_error(
          GameErrorCode::AssetInvalidPath,
          "asset path cannot be empty");
    }

    if (cache_.contains(relative_path))
    {
      return make_game_error(
          GameErrorCode::AssetAlreadyLoaded,
          "asset is already loaded");
    }

    dispatch_asset_event(EventType::AssetRequested, relative_path);

    const AssetId id = next_id();

    auto asset = load_asset_from_disk(relative_path, type, id);
    if (!asset)
    {
      dispatch_asset_event(EventType::AssetLoadFailed, relative_path);
      return asset.error();
    }

    auto stored = cache_.store(std::move(asset.value()));
    if (!stored)
    {
      dispatch_asset_event(EventType::AssetLoadFailed, relative_path);
      return stored.error();
    }

    dispatch_asset_event(EventType::AssetLoaded, relative_path);

    return id;
  }

  GameResult<AssetId> AssetManager::reload(const std::string &relative_path)
  {
    if (relative_path.empty())
    {
      return make_game_error(
          GameErrorCode::AssetInvalidPath,
          "asset path cannot be empty");
    }

    const bool was_loaded = cache_.contains(relative_path);

    if (was_loaded)
    {
      auto unloaded = unload(relative_path);
      if (!unloaded)
      {
        return unloaded.error();
      }
    }

    return load(relative_path);
  }

  GameBoolResult AssetManager::unload(AssetId id)
  {
    return cache_.remove(id);
  }

  GameBoolResult AssetManager::unload(const std::string &relative_path)
  {
    return cache_.remove(relative_path);
  }

  bool AssetManager::contains(AssetId id) const
  {
    return cache_.contains(id);
  }

  bool AssetManager::contains(const std::string &relative_path) const
  {
    return cache_.contains(relative_path);
  }

  Asset *AssetManager::get(AssetId id)
  {
    return cache_.get(id);
  }

  const Asset *AssetManager::get(AssetId id) const
  {
    return cache_.get(id);
  }

  Asset *AssetManager::get(const std::string &relative_path)
  {
    return cache_.get(relative_path);
  }

  const Asset *AssetManager::get(const std::string &relative_path) const
  {
    return cache_.get(relative_path);
  }

  AssetId AssetManager::id_for(const std::string &relative_path) const
  {
    return cache_.id_for(relative_path);
  }

  std::size_t AssetManager::size() const noexcept
  {
    return cache_.size();
  }

  bool AssetManager::empty() const noexcept
  {
    return cache_.empty();
  }

  std::vector<AssetId> AssetManager::ids() const
  {
    return cache_.ids();
  }

  std::vector<std::string> AssetManager::paths() const
  {
    return cache_.paths();
  }

  AssetCacheStats AssetManager::stats() const
  {
    return cache_.stats();
  }

  AssetCache &AssetManager::cache() noexcept
  {
    return cache_;
  }

  const AssetCache &AssetManager::cache() const noexcept
  {
    return cache_;
  }

  void AssetManager::clear()
  {
    cache_.clear();
    next_id_ = 1;
  }

  GameResult<Asset> AssetManager::load_asset_from_disk(
      const std::string &relative_path,
      AssetType type,
      AssetId id)
  {
    auto asset_path = AssetPath::make(asset_root_, relative_path);
    if (!asset_path)
    {
      return asset_path.error();
    }

    auto exists = vix::fs::exists(asset_path.value().full());
    if (!exists)
    {
      return make_game_error(
          GameErrorCode::AssetLoadFailed,
          "failed to check asset existence");
    }

    if (!exists.value())
    {
      return make_game_error(
          GameErrorCode::AssetNotFound,
          "asset file not found");
    }

    auto is_file = vix::fs::is_file(asset_path.value().full());
    if (!is_file)
    {
      return make_game_error(
          GameErrorCode::AssetLoadFailed,
          "failed to check asset file type");
    }

    if (!is_file.value())
    {
      return make_game_error(
          GameErrorCode::AssetInvalidPath,
          "asset path is not a file");
    }

    if (is_text_asset(type))
    {
      auto text = vix::fs::read_text(asset_path.value().full());
      if (!text)
      {
        return make_game_error(
            GameErrorCode::AssetLoadFailed,
            "failed to read text asset");
      }

      return Asset::text(
          id,
          type,
          std::move(asset_path.value()),
          std::move(text.value()));
    }

    auto bytes = vix::fs::read_file(asset_path.value().full());
    if (!bytes)
    {
      return make_game_error(
          GameErrorCode::AssetLoadFailed,
          "failed to read binary asset");
    }

    return Asset::binary(
        id,
        type,
        std::move(asset_path.value()),
        std::move(bytes.value()));
  }

  void AssetManager::dispatch_asset_event(
      EventType type,
      const std::string &relative_path)
  {
    if (!events_)
    {
      return;
    }

    Event event(type);
    event.set_source("asset_manager");
    event.set_target(relative_path);
    event.set_field("path", relative_path);

    const AssetId id = cache_.id_for(relative_path);
    if (is_valid_asset_id(id))
    {
      event.set_field("asset_id", std::to_string(id));
    }

    (void)events_->dispatch(std::move(event));
  }

  AssetId AssetManager::next_id() noexcept
  {
    return next_id_++;
  }

} // namespace vix::game
