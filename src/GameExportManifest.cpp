/**
 *
 *  @file GameExportManifest.cpp
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

#include <vix/game/GameExportManifest.hpp>

#include <fstream>

#include <vix/game/GameError.hpp>
#include <vix/json/dumps.hpp>

namespace vix::game
{
  vix::json::Json GameExportManifest::to_json() const
  {
    vix::json::Json json;

    json["name"] = name;
    json["version"] = version;
    json["asset_root"] = asset_root;

    vix::json::Json asset_list = vix::json::Json::array();

    for (const auto &asset : assets)
    {
      asset_list.push_back(asset.to_json());
    }

    json["assets"] = asset_list;

    json["output_path"] = output_path;
    json["copied_files"] = copied_files;
    json["copied_directories"] = copied_directories;
    json["build_date"] = build_date;

    return json;
  }

  GameBoolResult GameExportManifest::save_file(
      const std::string &path) const
  {
    if (path.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "game export manifest file path cannot be empty");
    }

    std::ofstream file(path);
    if (!file)
    {
      return make_game_error(
          GameErrorCode::ConfigLoadFailed,
          "failed to open game export manifest file for writing");
    }

    file << vix::json::dumps(to_json());

    if (!file)
    {
      return make_game_error(
          GameErrorCode::ConfigLoadFailed,
          "failed to write game export manifest file");
    }

    return true;
  }

} // namespace vix::game
