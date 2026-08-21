/**
 *
 *  @file GameExportConfig.cpp
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

#include <vix/game/GameExportConfig.hpp>
#include <vix/game/GameError.hpp>

namespace vix::game
{
  namespace
  {
    bool safe_relative_path(const std::filesystem::path &path)
    {
      if (path.empty() || path.is_absolute())
      {
        return false;
      }
      for (const auto &part : path.lexically_normal())
      {
        if (part == "..")
        {
          return false;
        }
      }
      return true;
    }
  }

  GameBoolResult GameExportConfig::validate() const
  {
    if (project_root.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "game export project root cannot be empty");
    }

    if (output_directory.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "game export output directory cannot be empty");
    }

    if (name.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "game export name cannot be empty");
    }

    if (std::filesystem::path(name).has_parent_path() || name == "." || name == "..")
    {
      return make_game_error(GameErrorCode::InvalidArgument,
                             "game export name must be a single directory name");
    }

    if (!safe_relative_path(asset_directory) ||
        !safe_relative_path(source_directory) ||
        !safe_relative_path(package_file) ||
        !safe_relative_path(readme_file))
    {
      return make_game_error(GameErrorCode::InvalidArgument,
                             "game export project paths must be safe relative paths");
    }

    if (!std::filesystem::exists(project_root))
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "game export project root does not exist");
    }

    if (!std::filesystem::is_directory(project_root))
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "game export project root is not a directory");
    }

    return true;
  }

} // namespace vix::game
