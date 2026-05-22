/**
 *
 *  @file AppConfig.cpp
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

#include <vix/game/AppConfig.hpp>

#include <vix/fs/ReadText.hpp>
#include <vix/game/GameError.hpp>
#include <vix/json/convert.hpp>
#include <vix/json/dumps.hpp>
#include <vix/json/loads.hpp>

namespace vix::game
{
  AppConfig AppConfig::defaults()
  {
    return AppConfig{};
  }

  GameResult<AppConfig> AppConfig::load_file(const std::string &path)
  {
    if (path.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigNotFound,
          "config path cannot be empty");
    }

    auto content = vix::fs::read_text(path);
    if (!content)
    {
      return make_game_error(
          GameErrorCode::ConfigLoadFailed,
          "failed to read config file");
    }

    auto json = vix::json::try_loads(content.value());
    if (!json)
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "failed to parse config json");
    }

    return from_json(json.value());
  }

  GameResult<AppConfig> AppConfig::from_json(const vix::json::Json &json)
  {
    if (!json.is_object())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "config json must be an object");
    }

    AppConfig config;

    config.name = vix::json::get_or<std::string>(json, "name", config.name);
    config.title = vix::json::get_or<std::string>(json, "title", config.title);
    config.asset_root = vix::json::get_or<std::string>(json, "asset_root", config.asset_root);
    config.start_scene = vix::json::get_or<std::string>(json, "start_scene", config.start_scene);
    config.headless = vix::json::get_or<bool>(json, "headless", config.headless);
    config.window.headless = config.headless;

    if (json.contains("window") && json["window"].is_object())
    {
      const auto &window_json = json["window"];

      config.window.title = vix::json::get_or<std::string>(
          window_json,
          "title",
          config.window.title);

      config.window.width = vix::json::get_or<std::uint32_t>(
          window_json,
          "width",
          config.window.width);

      config.window.height = vix::json::get_or<std::uint32_t>(
          window_json,
          "height",
          config.window.height);

      config.window.fullscreen = vix::json::get_or<bool>(
          window_json,
          "fullscreen",
          config.window.fullscreen);

      config.window.resizable = vix::json::get_or<bool>(
          window_json,
          "resizable",
          config.window.resizable);

      config.window.visible = vix::json::get_or<bool>(
          window_json,
          "visible",
          config.window.visible);

      config.window.vsync = vix::json::get_or<bool>(
          window_json,
          "vsync",
          config.window.vsync);

      config.window.headless = config.headless;
    }
    config.log_startup = vix::json::get_or<bool>(json, "log_startup", config.log_startup);
    config.log_shutdown = vix::json::get_or<bool>(json, "log_shutdown", config.log_shutdown);

    const auto target_fps = vix::json::get_or<std::uint32_t>(
        json,
        "target_fps",
        config.time_step.target_fps);

    config.time_step.set_target_fps(target_fps);

    const auto fixed_delta_ms = vix::json::get_or<vix::time::Duration::rep>(
        json,
        "fixed_delta_ms",
        config.time_step.fixed_delta.count_ms());

    const auto max_frame_delta_ms = vix::json::get_or<vix::time::Duration::rep>(
        json,
        "max_frame_delta_ms",
        config.time_step.max_frame_delta.count_ms());

    config.time_step.fixed_delta = vix::time::Duration::milliseconds(fixed_delta_ms);
    config.time_step.max_frame_delta = vix::time::Duration::milliseconds(max_frame_delta_ms);
    config.time_step.fixed_update = vix::json::get_or<bool>(
        json,
        "fixed_update",
        config.time_step.fixed_update);

    auto valid = config.validate();
    if (!valid)
    {
      return valid.error();
    }

    return config;
  }

  vix::json::Json AppConfig::to_json() const
  {
    vix::json::Json json = vix::json::Json::object();

    vix::json::Json window_json = vix::json::Json::object();

    window_json["title"] = window.title;
    window_json["width"] = window.width;
    window_json["height"] = window.height;
    window_json["fullscreen"] = window.fullscreen;
    window_json["resizable"] = window.resizable;
    window_json["visible"] = window.visible;
    window_json["vsync"] = window.vsync;
    window_json["headless"] = window.headless;

    json["window"] = std::move(window_json);

    json["name"] = name;
    json["title"] = title;
    json["asset_root"] = asset_root;
    json["start_scene"] = start_scene;
    json["target_fps"] = time_step.target_fps;
    json["fixed_delta_ms"] = time_step.fixed_delta.count_ms();
    json["max_frame_delta_ms"] = time_step.max_frame_delta.count_ms();
    json["fixed_update"] = time_step.fixed_update;
    json["headless"] = headless;
    json["log_startup"] = log_startup;
    json["log_shutdown"] = log_shutdown;

    return json;
  }

  GameBoolResult AppConfig::validate() const
  {
    if (name.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "app name cannot be empty");
    }

    if (title.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "app title cannot be empty");
    }

    if (asset_root.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "asset root cannot be empty");
    }

    if (start_scene.empty())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "start scene cannot be empty");
    }

    if (time_step.fixed_update && time_step.fixed_delta.is_zero())
    {
      return make_game_error(
          GameErrorCode::ConfigInvalid,
          "fixed delta cannot be zero when fixed update is enabled");
    }

    if (!headless)
    {
      auto valid_window = window.validate();
      if (!valid_window)
      {
        return valid_window.error();
      }
    }

    return true;
  }

  AppConfig &AppConfig::set_target_fps(std::uint32_t fps) noexcept
  {
    time_step.set_target_fps(fps);
    return *this;
  }

  AppConfig &AppConfig::set_title(std::string value)
  {
    title = std::move(value);
    return *this;
  }

  AppConfig &AppConfig::set_asset_root(std::string value)
  {
    asset_root = std::move(value);
    return *this;
  }

  AppConfig &AppConfig::set_start_scene(std::string value)
  {
    start_scene = std::move(value);
    return *this;
  }

} // namespace vix::game
