/**
 *
 *  @file SceneSerializer.cpp
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

#include <vix/game/SceneSerializer.hpp>

#include <fstream>
#include <sstream>
#include <utility>

#include <vix/game/GameError.hpp>
#include <vix/game/SceneData.hpp>
#include <vix/json/dumps.hpp>
#include <vix/json/loads.hpp>

namespace vix::game
{
  vix::json::Json SceneSerializer::to_json(
      const SceneManager &manager)
  {
    return manager.to_data().to_json();
  }

  GameBoolResult SceneSerializer::from_json(
      SceneManager &manager,
      const vix::json::Json &json,
      SceneFactory factory)
  {
    if (!factory)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "scene factory cannot be empty");
    }

    auto data = SceneFileData::from_json(json);
    if (!data)
    {
      return data.error();
    }

    manager.clear();

    for (const auto &entry : data.value().scenes)
    {
      auto scene = factory(entry.name);
      if (!scene)
      {
        return make_game_error(
            GameErrorCode::SceneLoadFailed,
            "scene factory returned null");
      }

      auto added = manager.add(entry.name, std::move(scene));
      if (!added)
      {
        return added.error();
      }

      if (entry.loaded)
      {
        auto loaded = manager.load(entry.name);
        if (!loaded)
        {
          return loaded.error();
        }
      }
    }

    if (!data.value().active_scene.empty())
    {
      auto active = manager.set_active(data.value().active_scene);
      if (!active)
      {
        return active.error();
      }
    }

    if (json.contains("active_scene") && json["active_scene"].is_string())
    {
      const std::string active_name =
          json["active_scene"].template get<std::string>();

      if (!active_name.empty())
      {
        auto active = manager.set_active(active_name);
        if (!active)
        {
          return active.error();
        }
      }
    }

    return true;
  }

  GameBoolResult SceneSerializer::save_file(
      const SceneManager &manager,
      const std::string &path)
  {
    if (path.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "scene file path cannot be empty");
    }

    std::ofstream file(path);
    if (!file)
    {
      return make_game_error(
          GameErrorCode::ConfigLoadFailed,
          "failed to open scene file for writing");
    }

    file << vix::json::dumps(to_json(manager));
    if (!file)
    {
      return make_game_error(
          GameErrorCode::ConfigLoadFailed,
          "failed to write scene file");
    }

    return true;
  }

  GameBoolResult SceneSerializer::load_file(
      SceneManager &manager,
      const std::string &path,
      SceneFactory factory)
  {
    if (path.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigNotFound,
          "scene file path cannot be empty");
    }

    std::ifstream file(path);
    if (!file)
    {
      return make_game_error(
          GameErrorCode::ConfigNotFound,
          "scene file not found");
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    auto json = vix::json::try_loads(buffer.str());
    if (!json)
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "failed to parse scene json");
    }

    return from_json(manager, json.value(), std::move(factory));
  }

} // namespace vix::game
