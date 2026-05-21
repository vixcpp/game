/**
 *
 *  @file SceneData.cpp
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

#include <vix/game/SceneData.hpp>

#include <utility>

#include <vix/game/GameError.hpp>
#include <vix/json/convert.hpp>

namespace vix::game
{
  vix::json::Json SceneData::to_json() const
  {
    vix::json::Json json = vix::json::Json::object();

    json["id"] = id;
    json["name"] = name;
    json["loaded"] = loaded;
    json["active"] = active;

    return json;
  }

  GameResult<SceneData> SceneData::from_json(
      const vix::json::Json &json)
  {
    if (!json.is_object())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "scene data json must be an object");
    }

    SceneData data;

    data.id = vix::json::get_or<SceneId>(
        json,
        "id",
        invalid_scene_id);

    data.name = vix::json::get_or<std::string>(
        json,
        "name",
        data.name);

    data.loaded = vix::json::get_or<bool>(
        json,
        "loaded",
        data.loaded);

    data.active = vix::json::get_or<bool>(
        json,
        "active",
        data.active);

    auto valid = data.validate();
    if (!valid)
    {
      return valid.error();
    }

    return data;
  }

  GameBoolResult SceneData::validate() const
  {
    if (name.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "scene name cannot be empty");
    }

    return true;
  }

  vix::json::Json SceneFileData::to_json() const
  {
    vix::json::Json json = vix::json::Json::object();

    json["version"] = version;
    json["active_scene"] = active_scene;

    vix::json::Json scene_array = vix::json::Json::array();
    for (const auto &scene : scenes)
    {
      scene_array.push_back(scene.to_json());
    }

    json["scenes"] = std::move(scene_array);

    return json;
  }

  GameResult<SceneFileData> SceneFileData::from_json(
      const vix::json::Json &json)
  {
    if (!json.is_object())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "scene file json must be an object");
    }

    SceneFileData data;

    data.version = vix::json::get_or<std::uint32_t>(
        json,
        "version",
        data.version);

    data.active_scene = vix::json::get_or<std::string>(
        json,
        "active_scene",
        data.active_scene);

    if (!json.contains("scenes") || !json["scenes"].is_array())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "scene file json must contain a scenes array");
    }

    for (const auto &entry : json["scenes"])
    {
      auto scene = SceneData::from_json(entry);
      if (!scene)
      {
        return scene.error();
      }

      data.scenes.push_back(std::move(scene.value()));
    }

    auto valid = data.validate();
    if (!valid)
    {
      return valid.error();
    }

    return data;
  }

  GameBoolResult SceneFileData::validate() const
  {
    if (version == 0)
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "scene file version cannot be zero");
    }

    for (const auto &scene : scenes)
    {
      auto valid = scene.validate();
      if (!valid)
      {
        return valid.error();
      }
    }

    return true;
  }

} // namespace vix::game
