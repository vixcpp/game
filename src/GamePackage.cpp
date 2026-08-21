/**
 *
 *  @file GamePackage.cpp
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

#include <vix/game/GamePackage.hpp>

#include <fstream>
#include <filesystem>
#include <sstream>
#include <utility>

#include <vix/game/GameError.hpp>
#include <vix/json/dumps.hpp>
#include <vix/json/loads.hpp>

namespace vix::game
{
  namespace
  {
    bool safe_relative_path(const std::string &value)
    {
      const std::filesystem::path path(value);
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

  GamePackage GamePackage::defaults()
  {
    GamePackage package;
    package.name = "vix-game";
    package.version = "0.1.0";
    package.entry_scene = "main";
    package.source_root = "src";
    package.asset_root = "assets";
    package.output_dir = "dist";
    package.scenes = {"main"};
    return package;
  }

  GameBoolResult GamePackage::validate() const
  {
    if (name.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "game package name cannot be empty");
    }

    if (version.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "game package version cannot be empty");
    }

    if (entry_scene.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "game package entry scene cannot be empty");
    }

    if (!safe_relative_path(source_root))
    {
      return make_game_error(GameErrorCode::ConfigInvalid,
                             "game package source root must be a safe relative path");
    }

    if (!safe_relative_path(asset_root))
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "game package asset root must be a safe relative path");
    }

    if (!safe_relative_path(output_dir))
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "game package output directory must be a safe relative path");
    }

    return true;
  }

  vix::json::Json GamePackage::to_json() const
  {
    vix::json::Json json;

    json["name"] = name;
    json["version"] = version;
    json["author"] = author;
    json["entry_scene"] = entry_scene;
    json["source_root"] = source_root;
    json["asset_root"] = asset_root;
    json["output_dir"] = output_dir;
    json["scenes"] = scenes;
    json["assets"] = assets;

    return json;
  }

  GameResult<GamePackage> GamePackage::from_json(
      const vix::json::Json &json)
  {
    if (!json.is_object())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "game package json must be an object");
    }

    GamePackage package = GamePackage::defaults();

    if (json.contains("name") && json["name"].is_string())
    {
      package.name = json["name"].template get<std::string>();
    }

    if (json.contains("version") && json["version"].is_string())
    {
      package.version = json["version"].template get<std::string>();
    }

    if (json.contains("author") && json["author"].is_string())
    {
      package.author = json["author"].template get<std::string>();
    }

    if (json.contains("entry_scene") && json["entry_scene"].is_string())
    {
      package.entry_scene = json["entry_scene"].template get<std::string>();
    }

    if (json.contains("source_root") && json["source_root"].is_string())
    {
      package.source_root = json["source_root"].template get<std::string>();
    }

    if (json.contains("asset_root") && json["asset_root"].is_string())
    {
      package.asset_root = json["asset_root"].template get<std::string>();
    }

    if (json.contains("output_dir") && json["output_dir"].is_string())
    {
      package.output_dir = json["output_dir"].template get<std::string>();
    }

    if (json.contains("scenes") && json["scenes"].is_array())
    {
      package.scenes.clear();

      for (const auto &item : json["scenes"])
      {
        if (item.is_string())
        {
          package.scenes.push_back(item.template get<std::string>());
        }
      }
    }

    if (json.contains("assets") && json["assets"].is_array())
    {
      package.assets.clear();

      for (const auto &item : json["assets"])
      {
        if (item.is_string())
        {
          package.assets.push_back(item.template get<std::string>());
        }
      }
    }

    auto valid = package.validate();
    if (!valid)
    {
      return valid.error();
    }

    return package;
  }

  GameBoolResult GamePackage::save_file(const std::string &path) const
  {
    if (path.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "game package file path cannot be empty");
    }

    auto valid = validate();
    if (!valid)
    {
      return valid.error();
    }

    std::ofstream file(path);
    if (!file)
    {
      return make_game_error(
          GameErrorCode::ConfigLoadFailed,
          "failed to open game package file for writing");
    }

    file << vix::json::dumps(to_json());

    if (!file)
    {
      return make_game_error(
          GameErrorCode::ConfigLoadFailed,
          "failed to write game package file");
    }

    return true;
  }

  GameResult<GamePackage> GamePackage::load_file(
      const std::string &path)
  {
    if (path.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigNotFound,
          "game package file path cannot be empty");
    }

    std::ifstream file(path);
    if (!file)
    {
      return make_game_error(
          GameErrorCode::ConfigNotFound,
          "game package file not found");
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    auto json = vix::json::try_loads(buffer.str());
    if (!json)
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "failed to parse game package json");
    }

    return from_json(json.value());
  }

} // namespace vix::game
