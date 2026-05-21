/**
 *
 *  @file AudioRuntime.hpp
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
#ifndef VIX_GAME_AUDIO_RUNTIME_HPP
#define VIX_GAME_AUDIO_RUNTIME_HPP

#include <cstddef>
#include <string>
#include <unordered_map>

#include <vix/game/GameContext.hpp>
#include <vix/game/GameResult.hpp>

namespace vix::game
{
  /**
   * @brief Runtime foundation for audio.
   *
   * AudioRuntime is a dependency-free audio coordination layer.
   *
   * It does not play real sound yet. It stores audio sources and exposes a
   * stable API that future audio backends can implement.
   */
  class AudioRuntime
  {
  public:
    /**
     * @brief Audio source id.
     */
    using SourceId = std::uint64_t;

    /**
     * @brief Invalid audio source id.
     */
    static constexpr SourceId invalid_source_id = 0;

    /**
     * @brief Audio source state.
     */
    struct Source
    {
      SourceId id{invalid_source_id};
      std::string name{};
      std::string path{};
      float volume{1.0F};
      bool loop{false};
      bool playing{false};

      [[nodiscard]] bool valid() const noexcept
      {
        return id != invalid_source_id && !path.empty();
      }
    };

    AudioRuntime() = default;

    explicit AudioRuntime(GameContext &context) noexcept;

    AudioRuntime(const AudioRuntime &) = delete;
    AudioRuntime &operator=(const AudioRuntime &) = delete;

    ~AudioRuntime() = default;

    AudioRuntime &attach(GameContext &context) noexcept;

    void detach() noexcept;

    [[nodiscard]] bool attached() const noexcept;

    [[nodiscard]] GameContext &context();

    [[nodiscard]] const GameContext &context() const;

    [[nodiscard]] GameBoolResult init();

    void shutdown() noexcept;

    [[nodiscard]] bool initialized() const noexcept;

    [[nodiscard]] GameResult<SourceId> load(
        std::string name,
        std::string path);

    [[nodiscard]] GameBoolResult unload(SourceId id);

    [[nodiscard]] GameBoolResult play(SourceId id);

    [[nodiscard]] GameBoolResult stop(SourceId id);

    [[nodiscard]] GameBoolResult set_volume(SourceId id, float volume);

    [[nodiscard]] GameBoolResult set_loop(SourceId id, bool enabled);

    [[nodiscard]] Source *source(SourceId id);

    [[nodiscard]] const Source *source(SourceId id) const;

    [[nodiscard]] SourceId id_for(const std::string &name) const;

    [[nodiscard]] bool contains(SourceId id) const;

    [[nodiscard]] bool contains(const std::string &name) const;

    void update();

    void clear();

    [[nodiscard]] std::size_t size() const noexcept;

    [[nodiscard]] bool empty() const noexcept;

  private:
    [[nodiscard]] SourceId next_id() noexcept;

  private:
    GameContext *context_{nullptr};
    std::unordered_map<SourceId, Source> sources_{};
    std::unordered_map<std::string, SourceId> name_to_id_{};
    SourceId next_id_{1};
    bool initialized_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_AUDIO_RUNTIME_HPP
