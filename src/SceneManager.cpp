/**
 *
 *  @file SceneManager.cpp
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

#include <vix/game/SceneManager.hpp>
#include <vix/game/App.hpp>
#include <vix/game/EventBus.hpp>

namespace vix::game
{
  void SceneManager::attach(App &app)
  {
    app_ = &app;

    for (auto &[id, record] : scenes_)
    {
      (void)id;

      if (record.scene)
      {
        record.scene->on_attach(app);
      }
    }
  }

  void SceneManager::detach()
  {
    for (auto &[id, record] : scenes_)
    {
      (void)id;

      if (record.scene)
      {
        record.scene->on_detach();
      }
    }

    app_ = nullptr;
  }

  GameResult<SceneId> SceneManager::add(
      std::string name,
      std::unique_ptr<Scene> scene)
  {
    if (name.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "scene name cannot be empty");
    }

    if (!scene)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "scene cannot be null");
    }

    if (contains(name))
    {
      return make_game_error(
          GameErrorCode::SceneAlreadyExists,
          "scene already exists");
    }

    const SceneId id = next_id();

    scene->set_id(id);
    scene->set_name(name);

    if (app_)
    {
      scene->on_attach(*app_);
    }

    SceneRecord record;
    record.id = id;
    record.name = name;
    record.scene = std::move(scene);

    scenes_.emplace(id, std::move(record));
    name_to_id_.emplace(std::move(name), id);

    return id;
  }

  GameBoolResult SceneManager::remove(SceneId id)
  {
    auto *entry = record(id);
    if (!entry)
    {
      return make_game_error(
          GameErrorCode::SceneNotFound,
          "scene not found");
    }

    if (active_id_ == id)
    {
      if (entry->scene)
      {
        entry->scene->on_exit();
      }

      active_id_ = invalid_scene_id;
    }

    if (entry->scene && entry->scene->loaded())
    {
      auto unloaded = entry->scene->on_unload();
      if (!unloaded)
      {
        return make_game_error(
            GameErrorCode::SceneUnloadFailed,
            "failed to unload scene before removal");
      }
    }

    if (entry->scene)
    {
      entry->scene->on_detach();
    }

    name_to_id_.erase(entry->name);
    scenes_.erase(id);

    return true;
  }

  GameBoolResult SceneManager::remove(const std::string &name)
  {
    const SceneId id = id_for(name);
    if (!is_valid_scene_id(id))
    {
      return make_game_error(
          GameErrorCode::SceneNotFound,
          "scene not found");
    }

    return remove(id);
  }

  GameBoolResult SceneManager::load(SceneId id)
  {
    auto *entry = record(id);
    if (!entry || !entry->scene)
    {
      return make_game_error(
          GameErrorCode::SceneNotFound,
          "scene not found");
    }

    if (entry->scene->loaded())
    {
      return true;
    }

    auto loaded = entry->scene->on_load();
    if (!loaded)
    {
      return make_game_error(
          GameErrorCode::SceneLoadFailed,
          "failed to load scene");
    }

    dispatch_scene_event(EventType::SceneLoaded, *entry);

    return true;
  }

  GameBoolResult SceneManager::load(const std::string &name)
  {
    const SceneId id = id_for(name);
    if (!is_valid_scene_id(id))
    {
      return make_game_error(
          GameErrorCode::SceneNotFound,
          "scene not found");
    }

    return load(id);
  }

  GameBoolResult SceneManager::unload(SceneId id)
  {
    auto *entry = record(id);
    if (!entry || !entry->scene)
    {
      return make_game_error(
          GameErrorCode::SceneNotFound,
          "scene not found");
    }

    if (active_id_ == id)
    {
      entry->scene->on_exit();
      active_id_ = invalid_scene_id;
    }

    if (!entry->scene->loaded())
    {
      return true;
    }

    auto unloaded = entry->scene->on_unload();
    if (!unloaded)
    {
      return make_game_error(
          GameErrorCode::SceneUnloadFailed,
          "failed to unload scene");
    }

    dispatch_scene_event(EventType::SceneUnloaded, *entry);

    return true;
  }

  GameBoolResult SceneManager::unload(const std::string &name)
  {
    const SceneId id = id_for(name);
    if (!is_valid_scene_id(id))
    {
      return make_game_error(
          GameErrorCode::SceneNotFound,
          "scene not found");
    }

    return unload(id);
  }

  GameBoolResult SceneManager::set_active(SceneId id)
  {
    auto *entry = record(id);
    if (!entry || !entry->scene)
    {
      return make_game_error(
          GameErrorCode::SceneNotFound,
          "scene not found");
    }

    if (active_id_ == id)
    {
      return true;
    }

    auto loaded = load(id);
    if (!loaded)
    {
      return loaded.error();
    }

    if (is_valid_scene_id(active_id_))
    {
      auto *current = record(active_id_);
      if (current && current->scene)
      {
        current->scene->on_exit();
      }
    }

    active_id_ = id;
    entry->scene->on_enter();

    dispatch_scene_event(EventType::SceneChanged, *entry);

    return true;
  }

  GameBoolResult SceneManager::set_active(const std::string &name)
  {
    const SceneId id = id_for(name);
    if (!is_valid_scene_id(id))
    {
      return make_game_error(
          GameErrorCode::SceneNotFound,
          "scene not found");
    }

    return set_active(id);
  }

  void SceneManager::update(const Frame &frame)
  {
    auto *scene = active();
    if (scene)
    {
      scene->on_update(frame);
    }
  }

  void SceneManager::fixed_update(const Frame &frame)
  {
    auto *scene = active();
    if (scene)
    {
      scene->on_fixed_update(frame);
    }
  }

  bool SceneManager::contains(SceneId id) const
  {
    return scenes_.find(id) != scenes_.end();
  }

  bool SceneManager::contains(const std::string &name) const
  {
    return name_to_id_.find(name) != name_to_id_.end();
  }

  SceneId SceneManager::id_for(const std::string &name) const
  {
    const auto it = name_to_id_.find(name);
    if (it == name_to_id_.end())
    {
      return invalid_scene_id;
    }

    return it->second;
  }

  Scene *SceneManager::get(SceneId id)
  {
    auto *entry = record(id);
    return entry ? entry->scene.get() : nullptr;
  }

  const Scene *SceneManager::get(SceneId id) const
  {
    const auto *entry = record(id);
    return entry ? entry->scene.get() : nullptr;
  }

  Scene *SceneManager::get(const std::string &name)
  {
    return get(id_for(name));
  }

  const Scene *SceneManager::get(const std::string &name) const
  {
    return get(id_for(name));
  }

  Scene *SceneManager::active()
  {
    return get(active_id_);
  }

  const Scene *SceneManager::active() const
  {
    return get(active_id_);
  }

  SceneId SceneManager::active_id() const noexcept
  {
    return active_id_;
  }

  std::string SceneManager::active_name() const
  {
    const auto *entry = record(active_id_);
    if (!entry)
    {
      return {};
    }

    return entry->name;
  }

  std::size_t SceneManager::size() const noexcept
  {
    return scenes_.size();
  }

  bool SceneManager::empty() const noexcept
  {
    return scenes_.empty();
  }

  std::vector<std::string> SceneManager::names() const
  {
    std::vector<std::string> out;
    out.reserve(name_to_id_.size());

    for (const auto &[name, id] : name_to_id_)
    {
      (void)id;
      out.push_back(name);
    }

    return out;
  }

  void SceneManager::clear()
  {
    if (is_valid_scene_id(active_id_))
    {
      auto *entry = record(active_id_);
      if (entry && entry->scene)
      {
        entry->scene->on_exit();
      }

      active_id_ = invalid_scene_id;
    }

    for (auto &[id, entry] : scenes_)
    {
      (void)id;

      if (!entry.scene)
      {
        continue;
      }

      if (entry.scene->loaded())
      {
        (void)entry.scene->on_unload();
      }

      entry.scene->on_detach();
    }

    scenes_.clear();
    name_to_id_.clear();
    next_id_ = 1;
  }

  SceneManager::SceneRecord *SceneManager::record(SceneId id)
  {
    const auto it = scenes_.find(id);
    if (it == scenes_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  const SceneManager::SceneRecord *SceneManager::record(SceneId id) const
  {
    const auto it = scenes_.find(id);
    if (it == scenes_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  void SceneManager::dispatch_scene_event(
      EventType type,
      const SceneRecord &entry)
  {
    if (!app_)
    {
      return;
    }

    Event event(type);
    event.set_source("scene_manager");
    event.set_target(entry.name);
    event.set_field("scene_id", std::to_string(entry.id));
    event.set_field("scene_name", entry.name);

    (void)app_->events().dispatch(std::move(event));
  }

  SceneId SceneManager::next_id() noexcept
  {
    return next_id_++;
  }

} // namespace vix::game
