/**
 *
 *  @file Asset.hpp
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
#ifndef VIX_GAME_ASSET_HPP
#define VIX_GAME_ASSET_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <vix/game/AssetId.hpp>
#include <vix/game/AssetPath.hpp>
#include <vix/game/AssetType.hpp>
#include <vix/time/Timestamp.hpp>

namespace vix::game
{
  /**
   * @brief Raw byte buffer used for binary assets.
   */
  using AssetBytes = std::vector<std::uint8_t>;

  /**
   * @brief Loaded asset metadata and content.
   *
   * Asset stores the V1 representation of an asset loaded by AssetManager.
   *
   * It can store:
   * - text content
   * - binary bytes
   * - metadata such as id, type, path, name, and load timestamp
   *
   * Higher-level systems can interpret the raw content later as textures,
   * sounds, scenes, shaders, scripts, or custom resources.
   */
  class Asset
  {
  public:
    /**
     * @brief Construct an empty invalid asset.
     */
    Asset() = default;

    /**
     * @brief Construct an asset with id, type, and path.
     *
     * @param id Asset id.
     * @param type Asset type.
     * @param path Asset path.
     */
    Asset(AssetId id, AssetType type, AssetPath path)
        : id_(id),
          type_(type),
          path_(std::move(path))
    {
      auto filename = path_.filename();
      if (filename)
      {
        name_ = filename.value();
      }
    }

    /**
     * @brief Create a text asset.
     *
     * @param id Asset id.
     * @param type Asset type.
     * @param path Asset path.
     * @param content Text content.
     * @return Asset object.
     */
    [[nodiscard]] static Asset text(
        AssetId id,
        AssetType type,
        AssetPath path,
        std::string content)
    {
      Asset asset(id, type, std::move(path));
      asset.text_ = std::move(content);
      asset.binary_ = false;
      asset.loaded_ = true;
      asset.loaded_at_ = vix::time::Timestamp::now();
      return asset;
    }

    /**
     * @brief Create a binary asset.
     *
     * @param id Asset id.
     * @param type Asset type.
     * @param path Asset path.
     * @param bytes Binary content.
     * @return Asset object.
     */
    [[nodiscard]] static Asset binary(
        AssetId id,
        AssetType type,
        AssetPath path,
        AssetBytes bytes)
    {
      Asset asset(id, type, std::move(path));
      asset.bytes_ = std::move(bytes);
      asset.binary_ = true;
      asset.loaded_ = true;
      asset.loaded_at_ = vix::time::Timestamp::now();
      return asset;
    }

    /**
     * @brief Return the asset id.
     */
    [[nodiscard]] AssetId id() const noexcept
    {
      return id_;
    }

    /**
     * @brief Set the asset id.
     *
     * Usually called by AssetManager.
     *
     * @param id Asset id.
     */
    void set_id(AssetId id) noexcept
    {
      id_ = id;
    }

    /**
     * @brief Return true if the asset has a valid id.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return is_valid_asset_id(id_);
    }

    /**
     * @brief Return the asset type.
     */
    [[nodiscard]] AssetType type() const noexcept
    {
      return type_;
    }

    /**
     * @brief Set the asset type.
     *
     * @param type Asset type.
     * @return Reference to this asset.
     */
    Asset &set_type(AssetType type) noexcept
    {
      type_ = type;
      return *this;
    }

    /**
     * @brief Return the asset path.
     */
    [[nodiscard]] const AssetPath &path() const noexcept
    {
      return path_;
    }

    /**
     * @brief Set the asset path.
     *
     * @param path Asset path.
     * @return Reference to this asset.
     */
    Asset &set_path(AssetPath path)
    {
      path_ = std::move(path);
      return *this;
    }

    /**
     * @brief Return the asset name.
     */
    [[nodiscard]] const std::string &name() const noexcept
    {
      return name_;
    }

    /**
     * @brief Set the asset name.
     *
     * @param value Asset name.
     * @return Reference to this asset.
     */
    Asset &set_name(std::string value)
    {
      name_ = std::move(value);
      return *this;
    }

    /**
     * @brief Return true if the asset is loaded.
     */
    [[nodiscard]] bool loaded() const noexcept
    {
      return loaded_;
    }

    /**
     * @brief Return true if the asset contains binary data.
     */
    [[nodiscard]] bool binary() const noexcept
    {
      return binary_;
    }

    /**
     * @brief Return true if the asset contains text data.
     */
    [[nodiscard]] bool textual() const noexcept
    {
      return !binary_;
    }

    /**
     * @brief Return text content.
     *
     * Empty when the asset is binary or has no text content.
     */
    [[nodiscard]] const std::string &text_content() const noexcept
    {
      return text_;
    }

    /**
     * @brief Return binary content.
     *
     * Empty when the asset is textual or has no binary content.
     */
    [[nodiscard]] const AssetBytes &bytes() const noexcept
    {
      return bytes_;
    }

    /**
     * @brief Return the asset size in bytes.
     */
    [[nodiscard]] std::size_t size() const noexcept
    {
      return binary_ ? bytes_.size() : text_.size();
    }

    /**
     * @brief Return true if the asset has no content.
     */
    [[nodiscard]] bool empty() const noexcept
    {
      return size() == 0;
    }

    /**
     * @brief Return the wall-clock timestamp when the asset was loaded.
     */
    [[nodiscard]] vix::time::Timestamp loaded_at() const noexcept
    {
      return loaded_at_;
    }

  private:
    /**
     * @brief Asset identifier.
     */
    AssetId id_{invalid_asset_id};

    /**
     * @brief Asset type.
     */
    AssetType type_{AssetType::Unknown};

    /**
     * @brief Asset path.
     */
    AssetPath path_{};

    /**
     * @brief Asset name.
     */
    std::string name_{};

    /**
     * @brief Text content.
     */
    std::string text_{};

    /**
     * @brief Binary content.
     */
    AssetBytes bytes_{};

    /**
     * @brief Whether the asset contains binary data.
     */
    bool binary_{false};

    /**
     * @brief Whether the asset was loaded.
     */
    bool loaded_{false};

    /**
     * @brief Load timestamp.
     */
    vix::time::Timestamp loaded_at_{};
  };

} // namespace vix::game

#endif // VIX_GAME_ASSET_HPP
