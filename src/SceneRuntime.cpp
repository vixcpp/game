/**
 *
 *  @file SceneRuntime.cpp
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

#include <vix/game/SceneRuntime.hpp>

#include <utility>

namespace vix::game
{
  SceneRuntime::SceneRuntime(GameContext &context) noexcept
      : context_(&context)
  {
  }

  SceneRuntime &SceneRuntime::attach(GameContext &context) noexcept
  {
    context_ = &context;
    return *this;
  }

  void SceneRuntime::detach() noexcept
  {
    context_ = nullptr;
  }

  bool SceneRuntime::attached() const noexcept
  {
    return context_ != nullptr;
  }

  GameContext &SceneRuntime::context()
  {
    return *context_;
  }

  const GameContext &SceneRuntime::context() const
  {
    return *context_;
  }

  GameResult<SceneId> SceneRuntime::add(
      std::string name,
      std::unique_ptr<Scene> scene)
  {
    return context().scenes().add(std::move(name), std::move(scene));
  }

  GameBoolResult SceneRuntime::load(SceneId id)
  {
    return context().scenes().load(id);
  }

  GameBoolResult SceneRuntime::load(const std::string &name)
  {
    return context().scenes().load(name);
  }

  GameBoolResult SceneRuntime::activate(SceneId id)
  {
    return context().scenes().set_active(id);
  }

  GameBoolResult SceneRuntime::activate(const std::string &name)
  {
    return context().scenes().set_active(name);
  }

  GameBoolResult SceneRuntime::unload(SceneId id)
  {
    return context().scenes().unload(id);
  }

  GameBoolResult SceneRuntime::unload(const std::string &name)
  {
    return context().scenes().unload(name);
  }

  GameBoolResult SceneRuntime::remove(SceneId id)
  {
    return context().scenes().remove(id);
  }

  GameBoolResult SceneRuntime::remove(const std::string &name)
  {
    return context().scenes().remove(name);
  }

  Scene *SceneRuntime::active()
  {
    return context().scenes().active();
  }

  const Scene *SceneRuntime::active() const
  {
    return context().scenes().active();
  }

  SceneId SceneRuntime::active_id() const noexcept
  {
    return context().scenes().active_id();
  }

  std::string SceneRuntime::active_name() const
  {
    return context().scenes().active_name();
  }

  SceneFileData SceneRuntime::inspect() const
  {
    return context().scenes().to_data();
  }

  GameBoolResult SceneRuntime::save_file(const std::string &path) const
  {
    return SceneSerializer::save_file(context().scenes(), path);
  }

  GameBoolResult SceneRuntime::load_file(
      const std::string &path,
      SceneSerializer::SceneFactory factory)
  {
    return SceneSerializer::load_file(
        context().scenes(),
        path,
        std::move(factory));
  }

  void SceneRuntime::clear()
  {
    context().scenes().clear();
  }

} // namespace vix::game
