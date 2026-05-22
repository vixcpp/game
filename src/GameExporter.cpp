/**
 *
 *  @file GameExporter.cpp
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

#include <vix/game/GameExporter.hpp>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <system_error>

#include <vix/game/GameError.hpp>
#include <vix/game/GameExportManifest.hpp>
#include <vix/game/GamePackage.hpp>
#include <vix/game/GameAssetPipeline.hpp>

namespace
{
  std::string make_build_date()
  {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};

#if defined(_WIN32)
    gmtime_s(&tm, &time);
#else
    gmtime_r(&time, &tm);
#endif

    std::ostringstream stream;
    stream << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");

    return stream.str();
  }
}

namespace vix::game
{
  GameResult<GameExportResult> GameExporter::export_project(
      const GameExportConfig &config) const
  {
    auto valid = config.validate();
    if (!valid)
    {
      return valid.error();
    }

    auto export_name = config.name;
    auto export_version = std::string{"0.1.0"};
    auto output_directory = config.output_directory;
    auto asset_directory = config.asset_directory;

    const auto package_path = config.project_root / config.package_file;

    std::error_code package_ec;
    if (std::filesystem::exists(package_path, package_ec))
    {
      auto package = GamePackage::load_file(package_path.string());
      if (!package)
      {
        return package.error();
      }

      export_name = package.value().name;
      export_version = package.value().version;
      output_directory = package.value().output_dir;
      asset_directory = package.value().asset_root;
    }

    const auto export_root = config.project_root / output_directory / export_name;

    std::error_code ec;

    if (std::filesystem::exists(export_root, ec))
    {
      if (!config.overwrite)
      {
        return make_game_error(
            GameErrorCode::InvalidState,
            "game export output directory already exists");
      }

      std::filesystem::remove_all(export_root, ec);
      if (ec)
      {
        return make_game_error(
            GameErrorCode::InvalidState,
            ec.message());
      }
    }

    std::filesystem::create_directories(export_root, ec);
    if (ec)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          ec.message());
    }

    GameExportResult result;
    result.output_path = export_root;
    result.name = export_name;
    result.version = export_version;
    result.asset_root = asset_directory.string();
    std::vector<GameExportAsset> exported_assets;

    if (config.copy_assets)
    {
      GameAssetPipeline asset_pipeline;

      auto scanned_assets = asset_pipeline.scan_assets(
          config.project_root,
          asset_directory);

      if (!scanned_assets)
      {
        return scanned_assets.error();
      }

      exported_assets = scanned_assets.value();

      auto copied = copy_directory_if_exists(
          config.project_root / asset_directory,
          export_root / asset_directory);

      if (!copied)
      {
        return copied.error();
      }

      result.copied_files += copied.value().copied_files;
      result.copied_directories += copied.value().copied_directories;
    }
    if (config.copy_package_file)
    {
      auto copied = copy_file_if_exists(
          config.project_root / config.package_file,
          export_root / config.package_file);

      if (!copied)
      {
        return copied.error();
      }

      result.copied_files += copied.value();
    }

    if (config.copy_readme)
    {
      auto copied = copy_file_if_exists(
          config.project_root / config.readme_file,
          export_root / config.readme_file);

      if (!copied)
      {
        return copied.error();
      }

      result.copied_files += copied.value();
    }

    ++result.copied_files;

    GameExportManifest manifest;
    manifest.name = result.name;
    manifest.version = result.version;
    manifest.asset_root = result.asset_root;
    manifest.assets = exported_assets;
    manifest.output_path = result.output_path.string();
    manifest.copied_files = result.copied_files;
    manifest.copied_directories = result.copied_directories;
    manifest.build_date = make_build_date();

    auto manifest_saved = manifest.save_file(
        (export_root / "export.json").string());

    if (!manifest_saved)
    {
      return manifest_saved.error();
    }

    return result;
  }

  GameResult<std::uint64_t> GameExporter::copy_file_if_exists(
      const std::filesystem::path &from,
      const std::filesystem::path &to)
  {
    std::error_code ec;

    if (!std::filesystem::exists(from, ec))
    {
      return static_cast<std::uint64_t>(0);
    }

    if (!std::filesystem::is_regular_file(from, ec))
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "game export source path is not a file");
    }

    std::filesystem::create_directories(to.parent_path(), ec);
    if (ec)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          ec.message());
    }

    std::filesystem::copy_file(
        from,
        to,
        std::filesystem::copy_options::overwrite_existing,
        ec);

    if (ec)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          ec.message());
    }

    return static_cast<std::uint64_t>(1);
  }

  GameResult<GameExportResult> GameExporter::copy_directory_if_exists(
      const std::filesystem::path &from,
      const std::filesystem::path &to)
  {
    GameExportResult result;
    result.output_path = to;

    std::error_code ec;

    if (!std::filesystem::exists(from, ec))
    {
      return result;
    }

    if (!std::filesystem::is_directory(from, ec))
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "game export source path is not a directory");
    }

    std::filesystem::create_directories(to, ec);
    if (ec)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          ec.message());
    }

    ++result.copied_directories;

    for (const auto &entry : std::filesystem::recursive_directory_iterator(from, ec))
    {
      if (ec)
      {
        return make_game_error(
            GameErrorCode::InvalidState,
            ec.message());
      }

      const auto relative = std::filesystem::relative(entry.path(), from, ec);
      if (ec)
      {
        return make_game_error(
            GameErrorCode::InvalidState,
            ec.message());
      }

      const auto target = to / relative;

      if (entry.is_directory(ec))
      {
        std::filesystem::create_directories(target, ec);
        if (ec)
        {
          return make_game_error(
              GameErrorCode::InvalidState,
              ec.message());
        }

        ++result.copied_directories;
      }
      else if (entry.is_regular_file(ec))
      {
        std::filesystem::create_directories(target.parent_path(), ec);
        if (ec)
        {
          return make_game_error(
              GameErrorCode::InvalidState,
              ec.message());
        }

        std::filesystem::copy_file(
            entry.path(),
            target,
            std::filesystem::copy_options::overwrite_existing,
            ec);

        if (ec)
        {
          return make_game_error(
              GameErrorCode::InvalidState,
              ec.message());
        }

        ++result.copied_files;
      }
    }

    return result;
  }

} // namespace vix::game
