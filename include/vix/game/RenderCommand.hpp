/**
 *
 *  @file RenderCommand.hpp
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
#ifndef VIX_GAME_RENDER_COMMAND_HPP
#define VIX_GAME_RENDER_COMMAND_HPP

#include <cstdint>

#include <vix/game/Color.hpp>

namespace vix::game
{
  /**
   * @brief Type of render command.
   *
   * RenderCommandType describes a backend-independent rendering operation.
   * V2 keeps this intentionally small. More command types can be added later
   * for textures, sprites, lines, meshes, text, and debug primitives.
   */
  enum class RenderCommandType : std::uint32_t
  {
    /**
     * @brief Unknown command.
     */
    Unknown = 0,

    /**
     * @brief Clear the current render target.
     */
    Clear,

    /**
     * @brief Begin a render frame.
     */
    BeginFrame,

    /**
     * @brief End a render frame.
     */
    EndFrame,

    /**
     * @brief Resize the render target or viewport.
     */
    Resize
  };

  /**
   * @brief Convert a render command type to a stable readable name.
   *
   * @param type Render command type.
   * @return Static command type name.
   */
  [[nodiscard]] inline constexpr const char *to_string(
      RenderCommandType type) noexcept
  {
    switch (type)
    {
    case RenderCommandType::Unknown:
      return "unknown";

    case RenderCommandType::Clear:
      return "clear";

    case RenderCommandType::BeginFrame:
      return "begin_frame";

    case RenderCommandType::EndFrame:
      return "end_frame";

    case RenderCommandType::Resize:
      return "resize";
    }

    return "unknown";
  }

  /**
   * @brief Backend-independent render command.
   *
   * RenderCommand is a small value object that describes one high-level render
   * operation. Renderer can execute these commands immediately or queue them
   * later when a command-buffer style API is introduced.
   */
  struct RenderCommand
  {
    /**
     * @brief Command type.
     */
    RenderCommandType type{RenderCommandType::Unknown};

    /**
     * @brief Clear color.
     *
     * Used when type == RenderCommandType::Clear.
     */
    Color color{Color::black()};

    /**
     * @brief Width value.
     *
     * Used when type == RenderCommandType::Resize.
     */
    std::uint32_t width{0};

    /**
     * @brief Height value.
     *
     * Used when type == RenderCommandType::Resize.
     */
    std::uint32_t height{0};

    /**
     * @brief Create a begin-frame command.
     *
     * @return Render command.
     */
    [[nodiscard]] static constexpr RenderCommand begin_frame() noexcept
    {
      RenderCommand command;
      command.type = RenderCommandType::BeginFrame;
      return command;
    }

    /**
     * @brief Create an end-frame command.
     *
     * @return Render command.
     */
    [[nodiscard]] static constexpr RenderCommand end_frame() noexcept
    {
      RenderCommand command;
      command.type = RenderCommandType::EndFrame;
      return command;
    }

    /**
     * @brief Create a clear command.
     *
     * @param value Clear color.
     * @return Render command.
     */
    [[nodiscard]] static constexpr RenderCommand clear(
        Color value) noexcept
    {
      RenderCommand command;
      command.type = RenderCommandType::Clear;
      command.color = value;
      return command;
    }

    /**
     * @brief Create a resize command.
     *
     * @param w New width.
     * @param h New height.
     * @return Render command.
     */
    [[nodiscard]] static constexpr RenderCommand resize(
        std::uint32_t w,
        std::uint32_t h) noexcept
    {
      RenderCommand command;
      command.type = RenderCommandType::Resize;
      command.width = w;
      command.height = h;
      return command;
    }

    /**
     * @brief Return true if this command is valid.
     */
    [[nodiscard]] constexpr bool valid() const noexcept
    {
      if (type == RenderCommandType::Unknown)
      {
        return false;
      }

      if (type == RenderCommandType::Resize)
      {
        return width > 0 && height > 0;
      }

      return true;
    }

    /**
     * @brief Return true if this command starts a frame.
     */
    [[nodiscard]] constexpr bool begins_frame() const noexcept
    {
      return type == RenderCommandType::BeginFrame;
    }

    /**
     * @brief Return true if this command ends a frame.
     */
    [[nodiscard]] constexpr bool ends_frame() const noexcept
    {
      return type == RenderCommandType::EndFrame;
    }
  };

} // namespace vix::game

#endif // VIX_GAME_RENDER_COMMAND_HPP
