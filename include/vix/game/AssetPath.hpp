/**
 *
 *  @file AssetPath.hpp
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
#ifndef VIX_GAME_ASSET_PATH_HPP
#define VIX_GAME_ASSET_PATH_HPP

#include <string>
#include <utility>

#include <vix/game/GameError.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/path/Extension.hpp>
#include <vix/path/Filename.hpp>
#include <vix/path/Join.hpp>
#include <vix/path/Normalize.hpp>

namespace vix::game
{
  /**
   * @brief Normalized path wrapper for game assets.
   *
   * AssetPath stores:
   * - the asset root directory
   * - the relative asset path
   * - the resolved full path
   *
   * It uses vix::path for lexical path operations and does not access the
   * filesystem directly.
   */
  class AssetPath
  {
  public:
    /**
     * @brief Construct an empty asset path.
     */
    AssetPath() = default;

    /**
     * @brief Construct an asset path from root and relative path.
     *
     * @param root Asset root directory.
     * @param relative Relative asset path.
     */
    AssetPath(std::string root, std::string relative)
        : root_(std::move(root)),
          relative_(std::move(relative))
    {
      rebuild_full_path();
    }

    /**
     * @brief Build and validate an asset path.
     *
     * @param root Asset root directory.
     * @param relative Relative asset path.
     * @return AssetPath or a structured error.
     */
    [[nodiscard]] static GameResult<AssetPath> make(
        std::string root,
        std::string relative)
    {
      if (root.empty())
      {
        return make_game_error(
            GameErrorCode::AssetInvalidPath,
            "asset root cannot be empty");
      }

      if (relative.empty())
      {
        return make_game_error(
            GameErrorCode::AssetInvalidPath,
            "asset relative path cannot be empty");
      }

      AssetPath path(std::move(root), std::move(relative));

      auto valid = path.validate();
      if (!valid)
      {
        return valid.error();
      }

      return path;
    }

    /**
     * @brief Return the asset root directory.
     */
    [[nodiscard]] const std::string &root() const noexcept
    {
      return root_;
    }

    /**
     * @brief Return the relative asset path.
     */
    [[nodiscard]] const std::string &relative() const noexcept
    {
      return relative_;
    }

    /**
     * @brief Return the resolved full asset path.
     */
    [[nodiscard]] const std::string &full() const noexcept
    {
      return full_;
    }

    /**
     * @brief Return true if the asset path is non-empty and valid.
     */
    [[nodiscard]] bool valid() const
    {
      return validate().ok();
    }

    /**
     * @brief Validate the asset path.
     *
     * @return true if valid, otherwise a structured error.
     */
    [[nodiscard]] GameBoolResult validate() const
    {
      if (root_.empty())
      {
        return make_game_error(
            GameErrorCode::AssetInvalidPath,
            "asset root cannot be empty");
      }

      if (relative_.empty())
      {
        return make_game_error(
            GameErrorCode::AssetInvalidPath,
            "asset relative path cannot be empty");
      }

      if (full_.empty())
      {
        return make_game_error(
            GameErrorCode::AssetInvalidPath,
            "asset full path cannot be empty");
      }

      return true;
    }

    /**
     * @brief Set the asset root directory.
     *
     * @param value New root.
     * @return Reference to this asset path.
     */
    AssetPath &set_root(std::string value)
    {
      root_ = std::move(value);
      rebuild_full_path();
      return *this;
    }

    /**
     * @brief Set the relative asset path.
     *
     * @param value New relative path.
     * @return Reference to this asset path.
     */
    AssetPath &set_relative(std::string value)
    {
      relative_ = std::move(value);
      rebuild_full_path();
      return *this;
    }

    /**
     * @brief Return the asset filename.
     *
     * @return Filename or a structured error.
     */
    [[nodiscard]] GameStringResult filename() const
    {
      auto result = vix::path::filename(relative_);
      if (!result)
      {
        return make_game_error(
            GameErrorCode::AssetInvalidPath,
            "failed to get asset filename");
      }

      return result.value();
    }

    /**
     * @brief Return the asset extension.
     *
     * @return Extension or a structured error.
     */
    [[nodiscard]] GameStringResult extension() const
    {
      auto result = vix::path::extension(relative_);
      if (!result)
      {
        return make_game_error(
            GameErrorCode::AssetInvalidPath,
            "failed to get asset extension");
      }

      return result.value();
    }

    /**
     * @brief Compare two asset paths by full path.
     */
    [[nodiscard]] friend bool operator==(
        const AssetPath &lhs,
        const AssetPath &rhs) noexcept
    {
      return lhs.full_ == rhs.full_;
    }

    /**
     * @brief Compare two asset paths by full path.
     */
    [[nodiscard]] friend bool operator!=(
        const AssetPath &lhs,
        const AssetPath &rhs) noexcept
    {
      return !(lhs == rhs);
    }

  private:
    /**
     * @brief Rebuild the full path from root and relative path.
     */
    void rebuild_full_path()
    {
      if (root_.empty() && relative_.empty())
      {
        full_.clear();
        return;
      }

      if (root_.empty())
      {
        auto normalized = vix::path::normalize(relative_);
        full_ = normalized ? normalized.value() : relative_;
        return;
      }

      if (relative_.empty())
      {
        auto normalized = vix::path::normalize(root_);
        full_ = normalized ? normalized.value() : root_;
        return;
      }

      auto joined = vix::path::join(root_, relative_);
      full_ = joined ? joined.value() : root_ + "/" + relative_;
    }

  private:
    /**
     * @brief Asset root directory.
     */
    std::string root_{};

    /**
     * @brief Relative asset path.
     */
    std::string relative_{};

    /**
     * @brief Full resolved asset path.
     */
    std::string full_{};
  };

} // namespace vix::game

#endif // VIX_GAME_ASSET_PATH_HPP
