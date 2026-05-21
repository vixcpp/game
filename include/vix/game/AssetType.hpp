/**
 *
 *  @file AssetType.hpp
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
#ifndef VIX_GAME_ASSET_TYPE_HPP
#define VIX_GAME_ASSET_TYPE_HPP

#include <string_view>

namespace vix::game
{
  /**
   * @brief Built-in asset types recognized by vix/game.
   *
   * AssetType describes the semantic type of a loaded asset.
   * V1 does not implement rendering, audio, or scripting directly, but the
   * asset system can already classify files for higher-level systems.
   */
  enum class AssetType
  {
    /**
     * @brief Unknown or unclassified asset.
     */
    Unknown = 0,

    /**
     * @brief Plain text asset.
     */
    Text,

    /**
     * @brief JSON document.
     */
    Json,

    /**
     * @brief Binary asset.
     */
    Binary,

    /**
     * @brief Image or texture file.
     */
    Image,

    /**
     * @brief Audio file.
     */
    Audio,

    /**
     * @brief Scene description file.
     */
    Scene,

    /**
     * @brief Shader source or compiled shader file.
     */
    Shader,

    /**
     * @brief Script file.
     */
    Script,

    /**
     * @brief User-defined asset type.
     */
    Custom
  };

  /**
   * @brief Convert an asset type to a stable readable name.
   *
   * @param type Asset type.
   * @return Static asset type name.
   */
  [[nodiscard]] inline constexpr const char *to_string(AssetType type) noexcept
  {
    switch (type)
    {
    case AssetType::Unknown:
      return "unknown";
    case AssetType::Text:
      return "text";
    case AssetType::Json:
      return "json";
    case AssetType::Binary:
      return "binary";
    case AssetType::Image:
      return "image";
    case AssetType::Audio:
      return "audio";
    case AssetType::Scene:
      return "scene";
    case AssetType::Shader:
      return "shader";
    case AssetType::Script:
      return "script";
    case AssetType::Custom:
      return "custom";
    }

    return "unknown";
  }

  /**
   * @brief Infer an asset type from a file extension.
   *
   * @param extension File extension, with or without leading dot.
   * @return Inferred asset type.
   */
  [[nodiscard]] inline constexpr AssetType asset_type_from_extension(
      std::string_view extension) noexcept
  {
    if (!extension.empty() && extension.front() == '.')
    {
      extension.remove_prefix(1);
    }

    if (extension == "txt" ||
        extension == "md" ||
        extension == "csv")
    {
      return AssetType::Text;
    }

    if (extension == "json")
    {
      return AssetType::Json;
    }

    if (extension == "scene" ||
        extension == "vscene")
    {
      return AssetType::Scene;
    }

    if (extension == "png" ||
        extension == "jpg" ||
        extension == "jpeg" ||
        extension == "webp" ||
        extension == "bmp" ||
        extension == "tga")
    {
      return AssetType::Image;
    }

    if (extension == "wav" ||
        extension == "mp3" ||
        extension == "ogg" ||
        extension == "flac")
    {
      return AssetType::Audio;
    }

    if (extension == "vert" ||
        extension == "frag" ||
        extension == "glsl" ||
        extension == "spv")
    {
      return AssetType::Shader;
    }

    if (extension == "lua" ||
        extension == "js" ||
        extension == "ts" ||
        extension == "vix")
    {
      return AssetType::Script;
    }

    if (extension == "bin" ||
        extension == "dat")
    {
      return AssetType::Binary;
    }

    return AssetType::Unknown;
  }

  /**
   * @brief Check whether the asset type is textual.
   *
   * @param type Asset type.
   * @return true if the asset is usually loaded as text.
   */
  [[nodiscard]] inline constexpr bool is_text_asset(AssetType type) noexcept
  {
    return type == AssetType::Text ||
           type == AssetType::Json ||
           type == AssetType::Scene ||
           type == AssetType::Shader ||
           type == AssetType::Script;
  }

  /**
   * @brief Check whether the asset type is binary.
   *
   * @param type Asset type.
   * @return true if the asset is usually loaded as binary.
   */
  [[nodiscard]] inline constexpr bool is_binary_asset(AssetType type) noexcept
  {
    return type == AssetType::Binary ||
           type == AssetType::Image ||
           type == AssetType::Audio;
  }

} // namespace vix::game

#endif // VIX_GAME_ASSET_TYPE_HPP
