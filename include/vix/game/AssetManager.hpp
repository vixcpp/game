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
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <vix/fs/Exists.hpp>
#include <vix/fs/IsFile.hpp>
#include <vix/fs/ReadFile.hpp>
#include <vix/fs/ReadText.hpp>
#include <vix/game/Asset.hpp>
#include <vix/game/AssetId.hpp>
#include <vix/game/AssetPath.hpp>
#include <vix/game/AssetType.hpp>
#include <vix/game/Event.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/EventType.hpp>
#include <vix/game/GameError.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/log/GlobalLog.hpp>

namespace vix::game
{
  /**
   * @brief Loads, stores, retrieves, and unloads game assets.
   *
   * AssetManager is the V1 asset system for vix/game.
   *
   * It supports:
   * - text asset loading
   * - binary asset loading
   * - asset type inference from extension
   * - lookup by id or relative path
   * - simple event emission through EventBus
   *
   * It does not decode textures, audio, shaders, or scenes directly.
   * It only loads raw content and metadata.
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
    explicit AssetManager(std::string asset_root)
        : asset_root_(std::move(asset_root))
    {
    }

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
    void set_event_bus(EventBus *bus) noexcept
    {
      events_ = bus;
    }

    /**
     * @brief Return the current asset root.
     */
    [[nodiscard]] const std::string &asset_root() const noexcept
    {
      return asset_root_;
    }

    /**
     * @brief Set the asset root.
     *
     * @param value New asset root.
     * @return Reference to this manager.
     */
    AssetManager &set_asset_root(std::string value)
    {
      asset_root_ = std::move(value);
      return *this;
    }

    /**
     * @brief Load an asset by relative path.
     *
     * The asset type is inferred from the file extension.
     *
     * @param relative_path Path relative to asset_root().
     * @return Loaded asset id, or a structured error.
     */
    [[nodiscard]] GameResult<AssetId> load(const std::string &relative_path)
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

    /**
     * @brief Load an asset by relative path and explicit type.
     *
     * @param relative_path Path relative to asset_root().
     * @param type Asset type.
     * @return Loaded asset id, or a structured error.
     */
    [[nodiscard]] GameResult<AssetId> load_as(
        const std::string &relative_path,
        AssetType type)
    {
      if (relative_path.empty())
      {
        return make_game_error(
            GameErrorCode::AssetInvalidPath,
            "asset path cannot be empty");
      }

      if (path_to_id_.find(relative_path) != path_to_id_.end())
      {
        return make_game_error(
            GameErrorCode::AssetAlreadyLoaded,
            "asset is already loaded");
      }

      auto asset_path = AssetPath::make(asset_root_, relative_path);
      if (!asset_path)
      {
        return asset_path.error();
      }

      dispatch_asset_event(EventType::AssetRequested, relative_path);

      auto exists = vix::fs::exists(asset_path.value().full());
      if (!exists)
      {
        return make_game_error(
            GameErrorCode::AssetLoadFailed,
            "failed to check asset existence");
      }

      if (!exists.value())
      {
        dispatch_asset_event(EventType::AssetLoadFailed, relative_path);

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

      const AssetId id = next_id();

      if (is_text_asset(type))
      {
        auto text = vix::fs::read_text(asset_path.value().full());
        if (!text)
        {
          dispatch_asset_event(EventType::AssetLoadFailed, relative_path);

          return make_game_error(
              GameErrorCode::AssetLoadFailed,
              "failed to read text asset");
        }

        Asset asset = Asset::text(
            id,
            type,
            std::move(asset_path.value()),
            std::move(text.value()));

        store_asset(std::move(asset), relative_path);
        dispatch_asset_event(EventType::AssetLoaded, relative_path);

        return id;
      }

      auto bytes = vix::fs::read_file(asset_path.value().full());
      if (!bytes)
      {
        dispatch_asset_event(EventType::AssetLoadFailed, relative_path);

        return make_game_error(
            GameErrorCode::AssetLoadFailed,
            "failed to read binary asset");
      }

      Asset asset = Asset::binary(
          id,
          type,
          std::move(asset_path.value()),
          std::move(bytes.value()));

      store_asset(std::move(asset), relative_path);
      dispatch_asset_event(EventType::AssetLoaded, relative_path);

      return id;
    }

    /**
     * @brief Unload an asset by id.
     *
     * @param id Asset id.
     * @return true if the asset was unloaded.
     */
    [[nodiscard]] GameBoolResult unload(AssetId id)
    {
      if (!is_valid_asset_id(id))
      {
        return make_game_error(
            GameErrorCode::InvalidArgument,
            "asset id is invalid");
      }

      auto it = assets_.find(id);
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

    /**
     * @brief Unload an asset by relative path.
     *
     * @param relative_path Relative asset path.
     * @return true if the asset was unloaded.
     */
    [[nodiscard]] GameBoolResult unload(const std::string &relative_path)
    {
      const auto it = path_to_id_.find(relative_path);
      if (it == path_to_id_.end())
      {
        return make_game_error(
            GameErrorCode::AssetNotFound,
            "asset not found");
      }

      return unload(it->second);
    }

    /**
     * @brief Return true if an asset id exists.
     */
    [[nodiscard]] bool contains(AssetId id) const
    {
      return assets_.find(id) != assets_.end();
    }

    /**
     * @brief Return true if a relative asset path exists.
     */
    [[nodiscard]] bool contains(const std::string &relative_path) const
    {
      return path_to_id_.find(relative_path) != path_to_id_.end();
    }

    /**
     * @brief Return an asset by id.
     *
     * @param id Asset id.
     * @return Asset pointer, or nullptr if missing.
     */
    [[nodiscard]] Asset *get(AssetId id)
    {
      const auto it = assets_.find(id);
      if (it == assets_.end())
      {
        return nullptr;
      }

      return &it->second;
    }

    /**
     * @brief Return an asset by id.
     *
     * @param id Asset id.
     * @return Asset pointer, or nullptr if missing.
     */
    [[nodiscard]] const Asset *get(AssetId id) const
    {
      const auto it = assets_.find(id);
      if (it == assets_.end())
      {
        return nullptr;
      }

      return &it->second;
    }

    /**
     * @brief Return an asset by relative path.
     *
     * @param relative_path Relative asset path.
     * @return Asset pointer, or nullptr if missing.
     */
    [[nodiscard]] Asset *get(const std::string &relative_path)
    {
      const auto id = id_for(relative_path);
      if (!is_valid_asset_id(id))
      {
        return nullptr;
      }

      return get(id);
    }

    /**
     * @brief Return an asset by relative path.
     *
     * @param relative_path Relative asset path.
     * @return Asset pointer, or nullptr if missing.
     */
    [[nodiscard]] const Asset *get(const std::string &relative_path) const
    {
      const auto id = id_for(relative_path);
      if (!is_valid_asset_id(id))
      {
        return nullptr;
      }

      return get(id);
    }

    /**
     * @brief Return an asset id for a relative path.
     *
     * @param relative_path Relative asset path.
     * @return Asset id, or invalid_asset_id if missing.
     */
    [[nodiscard]] AssetId id_for(const std::string &relative_path) const
    {
      const auto it = path_to_id_.find(relative_path);
      if (it == path_to_id_.end())
      {
        return invalid_asset_id;
      }

      return it->second;
    }

    /**
     * @brief Return loaded asset count.
     */
    [[nodiscard]] std::size_t size() const noexcept
    {
      return assets_.size();
    }

    /**
     * @brief Return true if no asset is loaded.
     */
    [[nodiscard]] bool empty() const noexcept
    {
      return assets_.empty();
    }

    /**
     * @brief Return all loaded asset ids.
     */
    [[nodiscard]] std::vector<AssetId> ids() const
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

    /**
     * @brief Remove all loaded assets.
     */
    void clear()
    {
      assets_.clear();
      path_to_id_.clear();
      next_id_ = 1;
    }

  private:
    /**
     * @brief Store a loaded asset and update indexes.
     *
     * @param asset Asset object.
     * @param relative_path Relative path used for lookup.
     */
    void store_asset(Asset asset, const std::string &relative_path)
    {
      const AssetId id = asset.id();
      path_to_id_[relative_path] = id;
      assets_.emplace(id, std::move(asset));
    }

    /**
     * @brief Dispatch an asset event if an event bus is available.
     *
     * @param type Event type.
     * @param relative_path Relative asset path.
     */
    void dispatch_asset_event(
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

      (void)events_->dispatch(std::move(event));
    }

    /**
     * @brief Return next unique asset id.
     */
    [[nodiscard]] AssetId next_id() noexcept
    {
      return next_id_++;
    }

  private:
    /**
     * @brief Root directory used to resolve assets.
     */
    std::string asset_root_{"assets"};

    /**
     * @brief Event bus used for asset events.
     */
    EventBus *events_{nullptr};

    /**
     * @brief Assets by id.
     */
    std::unordered_map<AssetId, Asset> assets_{};

    /**
     * @brief Relative path to asset id index.
     */
    std::unordered_map<std::string, AssetId> path_to_id_{};

    /**
     * @brief Next asset id.
     */
    AssetId next_id_{1};
  };

} // namespace vix::game

#endif // VIX_GAME_ASSET_MANAGER_HPP
