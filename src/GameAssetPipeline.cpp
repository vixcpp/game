/**
 *
 *  @file GameAssetPipeline.cpp
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

#include <vix/game/GameAssetPipeline.hpp>

#include <algorithm>
#include <cctype>
#include <string>
#include <system_error>

#include <vix/game/GameError.hpp>

namespace
{
  std::string lower_copy(std::string value)
  {
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char ch)
        {
          return static_cast<char>(std::tolower(ch));
        });

    return value;
  }
}

namespace vix::game
{
  GameResult<std::vector<GameExportAsset>> GameAssetPipeline::scan_assets(
      const std::filesystem::path &project_root,
      const std::filesystem::path &asset_root) const
  {
    std::vector<GameExportAsset> assets;

    if (project_root.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "asset pipeline project root cannot be empty");
    }

    if (asset_root.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "asset pipeline asset root cannot be empty");
    }

    const auto root = project_root / asset_root;

    std::error_code ec;

    if (!std::filesystem::exists(root, ec))
    {
      return assets;
    }

    if (!std::filesystem::is_directory(root, ec))
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "asset pipeline asset root is not a directory");
    }

    std::filesystem::recursive_directory_iterator it(root, ec);
    const std::filesystem::recursive_directory_iterator end;

    if (ec)
    {
      return make_game_error(
          GameErrorCode::AssetLoadFailed,
          ec.message());
    }

    while (it != end)
    {
      const auto path = it->path();

      if (should_ignore(path))
      {
        if (it->is_directory(ec))
        {
          it.disable_recursion_pending();
        }

        it.increment(ec);
        if (ec)
        {
          return make_game_error(
              GameErrorCode::AssetLoadFailed,
              ec.message());
        }

        continue;
      }

      if (it->is_regular_file(ec))
      {
        const auto relative = std::filesystem::relative(path, root, ec);
        if (ec)
        {
          return make_game_error(
              GameErrorCode::AssetLoadFailed,
              ec.message());
        }

        const auto size = std::filesystem::file_size(path, ec);
        if (ec)
        {
          return make_game_error(
              GameErrorCode::AssetLoadFailed,
              ec.message());
        }

        GameExportAsset asset;
        asset.path = relative.generic_string();
        asset.type = detect_type(path);
        asset.size_bytes = static_cast<std::uint64_t>(size);

        assets.push_back(asset);
      }

      it.increment(ec);
      if (ec)
      {
        return make_game_error(
            GameErrorCode::AssetLoadFailed,
            ec.message());
      }
    }

    return assets;
  }

  GameExportAssetType GameAssetPipeline::detect_type(
      const std::filesystem::path &path)
  {
    const auto ext = lower_copy(path.extension().string());

    if (ext == ".png" ||
        ext == ".jpg" ||
        ext == ".jpeg" ||
        ext == ".bmp" ||
        ext == ".gif" ||
        ext == ".webp")
    {
      return GameExportAssetType::Image;
    }

    if (ext == ".txt" ||
        ext == ".md" ||
        ext == ".json" ||
        ext == ".xml" ||
        ext == ".csv" ||
        ext == ".ini" ||
        ext == ".cfg" ||
        ext == ".toml" ||
        ext == ".yaml" ||
        ext == ".yml" ||
        ext == ".glsl" ||
        ext == ".vert" ||
        ext == ".frag")
    {
      return GameExportAssetType::Text;
    }

    return GameExportAssetType::Binary;
  }

  bool GameAssetPipeline::should_ignore(
      const std::filesystem::path &path)
  {
    const auto filename = lower_copy(path.filename().string());
    const auto ext = lower_copy(path.extension().string());

    if (filename.empty())
    {
      return false;
    }

    if (filename == ".ds_store" ||
        filename == ".gitkeep" ||
        filename == "thumbs.db" ||
        filename == ".git" ||
        filename == ".vix" ||
        filename == "build" ||
        filename == "build-ninja" ||
        filename == "dist" ||
        filename == "cmakefiles" ||
        filename == "__pycache__" ||
        filename == "cache" ||
        filename == "tmp" ||
        filename == "temp")
    {
      return true;
    }

    if (ext == ".tmp" ||
        ext == ".cache" ||
        ext == ".log" ||
        ext == ".swp")
    {
      return true;
    }

    return false;
  }

} // namespace vix::game
