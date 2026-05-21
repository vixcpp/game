/**
 *
 *  @file Sprite.hpp
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
#ifndef VIX_GAME_SPRITE_HPP
#define VIX_GAME_SPRITE_HPP

#include <vix/game/Color.hpp>
#include <vix/game/Rect.hpp>
#include <vix/game/TextureHandle.hpp>
#include <vix/game/Vec2.hpp>

namespace vix::game
{
  /**
   * @brief 2D sprite component.
   *
   * Sprite describes how an entity should be drawn by a 2D renderer.
   *
   * It is designed to be used with Transform2D:
   *
   *   registry.emplace_component<Transform2D>(entity.id());
   *   registry.emplace_component<Sprite>(entity.id());
   *
   * Sprite does not own texture data. It only references a TextureHandle.
   */
  struct Sprite
  {
    /**
     * @brief Texture referenced by this sprite.
     */
    TextureHandle texture{TextureHandle::invalid()};

    /**
     * @brief Source rectangle inside the texture.
     *
     * If source is empty, renderers may use the full texture.
     */
    Rect source{Rect::empty()};

    /**
     * @brief Sprite size in world units.
     *
     * If size is zero, renderers may infer size from the texture.
     */
    Vec2 size{Vec2::zero()};

    /**
     * @brief Sprite tint color.
     */
    Color tint{Color::white()};

    /**
     * @brief Whether the sprite is visible.
     */
    bool visible{true};

    /**
     * @brief Whether the sprite should be flipped horizontally.
     */
    bool flip_x{false};

    /**
     * @brief Whether the sprite should be flipped vertically.
     */
    bool flip_y{false};

    /**
     * @brief Create a default visible white sprite.
     */
    constexpr Sprite() noexcept = default;

    /**
     * @brief Create a sprite from a texture handle.
     *
     * @param value Texture handle.
     */
    explicit constexpr Sprite(TextureHandle value) noexcept
        : texture(value)
    {
    }

    /**
     * @brief Create a sprite from a texture handle and size.
     *
     * @param texture_value Texture handle.
     * @param size_value Sprite size.
     */
    constexpr Sprite(
        TextureHandle texture_value,
        Vec2 size_value) noexcept
        : texture(texture_value),
          size(size_value)
    {
    }

    /**
     * @brief Create a sprite from an asset id.
     *
     * @param id Asset id.
     * @return Sprite.
     */
    [[nodiscard]] static constexpr Sprite from_asset(
        AssetId id) noexcept
    {
      return Sprite{TextureHandle::from_asset(id)};
    }

    /**
     * @brief Set the texture handle.
     *
     * @param value Texture handle.
     * @return Reference to this sprite.
     */
    constexpr Sprite &set_texture(TextureHandle value) noexcept
    {
      texture = value;
      return *this;
    }

    /**
     * @brief Set the texture from an asset id.
     *
     * @param id Asset id.
     * @return Reference to this sprite.
     */
    constexpr Sprite &set_texture_asset(AssetId id) noexcept
    {
      texture = TextureHandle::from_asset(id);
      return *this;
    }

    /**
     * @brief Set the source rectangle.
     *
     * @param value Source rectangle.
     * @return Reference to this sprite.
     */
    constexpr Sprite &set_source(Rect value) noexcept
    {
      source = value;
      return *this;
    }

    /**
     * @brief Set the sprite size.
     *
     * @param value Sprite size.
     * @return Reference to this sprite.
     */
    constexpr Sprite &set_size(Vec2 value) noexcept
    {
      size = value;
      return *this;
    }

    /**
     * @brief Set the sprite tint.
     *
     * @param value Tint color.
     * @return Reference to this sprite.
     */
    constexpr Sprite &set_tint(Color value) noexcept
    {
      tint = value;
      return *this;
    }

    /**
     * @brief Set sprite visibility.
     *
     * @param value Visibility flag.
     * @return Reference to this sprite.
     */
    constexpr Sprite &set_visible(bool value) noexcept
    {
      visible = value;
      return *this;
    }

    /**
     * @brief Set horizontal flip.
     *
     * @param value Flip flag.
     * @return Reference to this sprite.
     */
    constexpr Sprite &set_flip_x(bool value) noexcept
    {
      flip_x = value;
      return *this;
    }

    /**
     * @brief Set vertical flip.
     *
     * @param value Flip flag.
     * @return Reference to this sprite.
     */
    constexpr Sprite &set_flip_y(bool value) noexcept
    {
      flip_y = value;
      return *this;
    }

    /**
     * @brief Return true if the sprite has a valid texture.
     */
    [[nodiscard]] constexpr bool has_texture() const noexcept
    {
      return texture.valid();
    }

    /**
     * @brief Return true if the sprite has a valid explicit size.
     */
    [[nodiscard]] constexpr bool has_size() const noexcept
    {
      return !size.zero_value();
    }

    /**
     * @brief Return true if the sprite has a valid source rectangle.
     */
    [[nodiscard]] constexpr bool has_source() const noexcept
    {
      return source.valid();
    }

    /**
     * @brief Return true if this sprite can be rendered.
     */
    [[nodiscard]] constexpr bool renderable() const noexcept
    {
      return visible && has_texture();
    }

    /**
     * @brief Compare two sprites.
     */
    [[nodiscard]] friend constexpr bool operator==(
        const Sprite &lhs,
        const Sprite &rhs) noexcept
    {
      return lhs.texture == rhs.texture &&
             lhs.source == rhs.source &&
             lhs.size == rhs.size &&
             lhs.tint == rhs.tint &&
             lhs.visible == rhs.visible &&
             lhs.flip_x == rhs.flip_x &&
             lhs.flip_y == rhs.flip_y;
    }

    /**
     * @brief Compare two sprites.
     */
    [[nodiscard]] friend constexpr bool operator!=(
        const Sprite &lhs,
        const Sprite &rhs) noexcept
    {
      return !(lhs == rhs);
    }
  };

} // namespace vix::game

#endif // VIX_GAME_SPRITE_HPP
