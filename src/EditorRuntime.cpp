/**
 *
 *  @file EditorRuntime.cpp
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

#include <vix/game/EditorRuntime.hpp>
#include <vix/game/GameError.hpp>
#include <vix/game/Registry.hpp>
#include <vix/game/SceneData.hpp>

namespace vix::game
{
  EditorRuntime::EditorRuntime(GameRuntime &runtime) noexcept
      : runtime_(&runtime),
        context_(runtime.context()),
        initialized_(false)
  {
  }

  EditorRuntime &EditorRuntime::attach(GameRuntime &runtime) noexcept
  {
    runtime_ = &runtime;
    context_.attach(runtime.context());
    return *this;
  }

  void EditorRuntime::detach() noexcept
  {
    shutdown();
    context_.detach();
    runtime_ = nullptr;
  }

  bool EditorRuntime::attached() const noexcept
  {
    return runtime_ != nullptr && context_.attached();
  }

  GameBoolResult EditorRuntime::init()
  {
    if (initialized_)
    {
      return true;
    }

    if (!attached())
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "editor runtime is not attached to a game runtime");
    }

    auto runtime_init = runtime_->init();
    if (!runtime_init)
    {
      return runtime_init.error();
    }

    context_.enter_edit_mode();

    initialized_ = true;
    return true;
  }

  void EditorRuntime::shutdown() noexcept
  {
    if (!initialized_)
    {
      return;
    }

    context_.clear();
    initialized_ = false;
  }

  bool EditorRuntime::initialized() const noexcept
  {
    return initialized_;
  }

  void EditorRuntime::begin_frame(const Frame &frame)
  {
    if (runtime_)
    {
      runtime_->begin_frame(frame);
    }
  }

  void EditorRuntime::update(const Frame &frame)
  {
    if (!runtime_)
    {
      return;
    }

    if (context_.play_mode())
    {
      runtime_->update(frame);
      return;
    }

    (void)frame;
  }

  void EditorRuntime::render(const Frame &frame)
  {
    if (runtime_)
    {
      runtime_->render(frame);
    }
  }

  void EditorRuntime::end_frame(const Frame &frame)
  {
    if (runtime_)
    {
      runtime_->end_frame(frame);
    }
  }

  void EditorRuntime::enter_edit_mode() noexcept
  {
    context_.enter_edit_mode();
  }

  void EditorRuntime::enter_play_mode() noexcept
  {
    context_.enter_play_mode();
  }

  EditorContext &EditorRuntime::context() noexcept
  {
    return context_;
  }

  const EditorContext &EditorRuntime::context() const noexcept
  {
    return context_;
  }

  GameRuntime &EditorRuntime::game_runtime()
  {
    return *runtime_;
  }

  const GameRuntime &EditorRuntime::game_runtime() const
  {
    return *runtime_;
  }

  RuntimeDiagnostics EditorRuntime::inspect_runtime() const
  {
    if (runtime_ == nullptr)
    {
      return RuntimeDiagnostics{};
    }

    return runtime_->diagnostics();
  }

  SceneFileData EditorRuntime::inspect_scenes() const
  {
    if (runtime_ == nullptr)
    {
      return SceneFileData{};
    }

    return runtime_->context().scenes().to_data();
  }

  RegistryStats EditorRuntime::inspect_registry() const
  {
    if (runtime_ == nullptr)
    {
      return RegistryStats{};
    }

    const auto *scene = runtime_->context().scenes().active();
    if (scene == nullptr)
    {
      return RegistryStats{};
    }

    const auto *registry = scene->registry();
    if (registry == nullptr)
    {
      return RegistryStats{};
    }

    return registry->stats();
  }
} // namespace vix::game
