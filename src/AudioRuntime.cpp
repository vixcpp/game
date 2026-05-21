/**
 *
 *  @file AudioRuntime.cpp
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

#include <vix/game/AudioRuntime.hpp>

#include <algorithm>
#include <utility>

#include <vix/game/GameError.hpp>

namespace vix::game
{
  AudioRuntime::AudioRuntime(GameContext &context) noexcept
      : context_(&context),
        sources_(),
        name_to_id_(),
        next_id_(1),
        initialized_(false)
  {
  }

  AudioRuntime &AudioRuntime::attach(GameContext &context) noexcept
  {
    context_ = &context;
    return *this;
  }

  void AudioRuntime::detach() noexcept
  {
    shutdown();
    context_ = nullptr;
  }

  bool AudioRuntime::attached() const noexcept
  {
    return context_ != nullptr;
  }

  GameContext &AudioRuntime::context()
  {
    return *context_;
  }

  const GameContext &AudioRuntime::context() const
  {
    return *context_;
  }

  GameBoolResult AudioRuntime::init()
  {
    if (initialized_)
    {
      return true;
    }

    if (!attached())
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "audio runtime is not attached to a game context");
    }

    initialized_ = true;
    return true;
  }

  void AudioRuntime::shutdown() noexcept
  {
    if (!initialized_)
    {
      return;
    }

    clear();
    initialized_ = false;
  }

  bool AudioRuntime::initialized() const noexcept
  {
    return initialized_;
  }

  GameResult<AudioRuntime::SourceId> AudioRuntime::load(
      std::string name,
      std::string path)
  {
    if (name.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "audio source name cannot be empty");
    }

    if (path.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "audio source path cannot be empty");
    }

    if (contains(name))
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "audio source already exists");
    }

    const SourceId id = next_id();

    Source source;
    source.id = id;
    source.name = name;
    source.path = std::move(path);

    sources_.emplace(id, std::move(source));
    name_to_id_.emplace(std::move(name), id);

    return id;
  }

  GameBoolResult AudioRuntime::unload(SourceId id)
  {
    auto it = sources_.find(id);
    if (it == sources_.end())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "audio source not found");
    }

    name_to_id_.erase(it->second.name);
    sources_.erase(it);

    return true;
  }

  GameBoolResult AudioRuntime::play(SourceId id)
  {
    auto *item = source(id);
    if (!item)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "audio source not found");
    }

    item->playing = true;
    return true;
  }

  GameBoolResult AudioRuntime::stop(SourceId id)
  {
    auto *item = source(id);
    if (!item)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "audio source not found");
    }

    item->playing = false;
    return true;
  }

  GameBoolResult AudioRuntime::set_volume(SourceId id, float volume)
  {
    auto *item = source(id);
    if (!item)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "audio source not found");
    }

    item->volume = std::clamp(volume, 0.0F, 1.0F);
    return true;
  }

  GameBoolResult AudioRuntime::set_loop(SourceId id, bool enabled)
  {
    auto *item = source(id);
    if (!item)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "audio source not found");
    }

    item->loop = enabled;
    return true;
  }

  AudioRuntime::Source *AudioRuntime::source(SourceId id)
  {
    const auto it = sources_.find(id);
    if (it == sources_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  const AudioRuntime::Source *AudioRuntime::source(SourceId id) const
  {
    const auto it = sources_.find(id);
    if (it == sources_.end())
    {
      return nullptr;
    }

    return &it->second;
  }

  AudioRuntime::SourceId AudioRuntime::id_for(const std::string &name) const
  {
    const auto it = name_to_id_.find(name);
    if (it == name_to_id_.end())
    {
      return invalid_source_id;
    }

    return it->second;
  }

  bool AudioRuntime::contains(SourceId id) const
  {
    return sources_.find(id) != sources_.end();
  }

  bool AudioRuntime::contains(const std::string &name) const
  {
    return name_to_id_.find(name) != name_to_id_.end();
  }

  void AudioRuntime::update()
  {
  }

  void AudioRuntime::clear()
  {
    sources_.clear();
    name_to_id_.clear();
    next_id_ = 1;
  }

  std::size_t AudioRuntime::size() const noexcept
  {
    return sources_.size();
  }

  bool AudioRuntime::empty() const noexcept
  {
    return sources_.empty();
  }

  AudioRuntime::SourceId AudioRuntime::next_id() noexcept
  {
    return next_id_++;
  }

} // namespace vix::game
