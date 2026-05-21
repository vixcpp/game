/**
 *
 *  @file TextureHandle.hpp
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
#ifndef VIX_GAME_TEXTURE_HANDLE_HPP
#define VIX_GAME_TEXTURE_HANDLE_HPP

#include <cstdint>

#include <vix/game/AssetId.hpp>

namespace vix::game
{
  /**
   * @brief Backend-independent texture handle.
   *
   * TextureHandle does not own GPU resources directly.
   *
   * It is a small identifier object used by Sprite and Renderer2D to reference
   * a texture-like asset or a backend texture resource.
   *
   * In V2, it mainly maps to AssetId. Later render backends can attach their
   * own native texture ids internally.
   */
  struct TextureHandle
  {
    /**
     * @brief Texture identifier.
     */
    AssetId id{invalid_asset_id};

    /**
     * @brief Create an invalid texture handle.
     */
    constexpr TextureHandle() noexcept = default;

    /**
     * @brief Create a texture handle from an asset id.
     *
     * @param value Asset id.
     */
    explicit constexpr TextureHandle(AssetId value) noexcept
        : id(value)
    {
    }

    /**
     * @brief Return an invalid texture handle.
     */
    [[nodiscard]] static constexpr TextureHandle invalid() noexcept
    {
      return TextureHandle{};
    }

    /**
     * @brief Create a texture handle from an asset id.
     *
     * @param value Asset id.
     * @return Texture handle.
     */
    [[nodiscard]] static constexpr TextureHandle from_asset(
        AssetId value) noexcept
    {
      return TextureHandle{value};
    }

    /**
     * @brief Return true if the handle references a valid texture.
     */
    [[nodiscard]] constexpr bool valid() const noexcept
    {
      return is_valid_asset_id(id);
    }

    /**
     * @brief Return the underlying asset id.
     */
    [[nodiscard]] constexpr AssetId asset_id() const noexcept
    {
      return id;
    }

    /**
     * @brief Reset the handle to invalid.
     */
    constexpr void reset() noexcept
    {
      id = invalid_asset_id;
    }

    /**
     * @brief Compare two texture handles.
     */
    [[nodiscard]] friend constexpr bool operator==(
        const TextureHandle &lhs,
        const TextureHandle &rhs) noexcept
    {
      return lhs.id == rhs.id;
    }

    /**
     * @brief Compare two texture handles.
     */
    [[nodiscard]] friend constexpr bool operator!=(
        const TextureHandle &lhs,
        const TextureHandle &rhs) noexcept
    {
      return !(lhs == rhs);
    }
  };

} // namespace vix::game

#endif // VIX_GAME_TEXTURE_HANDLE_HPP
