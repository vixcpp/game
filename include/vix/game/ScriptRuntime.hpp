/**
 *
 *  @file ScriptRuntime.hpp
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
#ifndef VIX_GAME_SCRIPT_RUNTIME_HPP
#define VIX_GAME_SCRIPT_RUNTIME_HPP

#include <functional>
#include <string>
#include <unordered_map>

#include <vix/game/Frame.hpp>
#include <vix/game/GameContext.hpp>
#include <vix/game/GameResult.hpp>

namespace vix::game
{
  /**
   * @brief Foundation for future scripting integration.
   *
   * ScriptRuntime provides a small dependency-free scripting hook layer.
   *
   * It does not embed Lua, JavaScript, Python, or another scripting engine yet.
   * Instead, it gives vix/game a stable runtime API where future script engines
   * can be attached later.
   */
  class ScriptRuntime
  {
  public:
    /**
     * @brief Script callback type.
     */
    using ScriptCallback = std::function<void(GameContext &, const Frame &)>;

    /**
     * @brief Construct an empty script runtime.
     */
    ScriptRuntime() = default;

    /**
     * @brief Construct a script runtime attached to a game context.
     *
     * @param context Game context.
     */
    explicit ScriptRuntime(GameContext &context) noexcept;

    ScriptRuntime(const ScriptRuntime &) = delete;
    ScriptRuntime &operator=(const ScriptRuntime &) = delete;

    /**
     * @brief Destroy the script runtime.
     */
    ~ScriptRuntime() = default;

    /**
     * @brief Attach to a game context.
     *
     * @param context Game context.
     * @return Reference to this runtime.
     */
    ScriptRuntime &attach(GameContext &context) noexcept;

    /**
     * @brief Detach from the game context.
     */
    void detach() noexcept;

    /**
     * @brief Return true if attached to a game context.
     */
    [[nodiscard]] bool attached() const noexcept;

    /**
     * @brief Return the game context.
     */
    [[nodiscard]] GameContext &context();

    /**
     * @brief Return the game context.
     */
    [[nodiscard]] const GameContext &context() const;

    /**
     * @brief Initialize the script runtime.
     *
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult init();

    /**
     * @brief Shut down the script runtime.
     */
    void shutdown() noexcept;

    /**
     * @brief Return true if initialized.
     */
    [[nodiscard]] bool initialized() const noexcept;

    /**
     * @brief Register a named script callback.
     *
     * @param name Callback name.
     * @param callback Callback function.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult register_callback(
        std::string name,
        ScriptCallback callback);

    /**
     * @brief Remove a named script callback.
     *
     * @param name Callback name.
     * @return true if the callback existed and was removed.
     */
    [[nodiscard]] GameBoolResult unregister_callback(
        const std::string &name);

    /**
     * @brief Return true if a callback exists.
     *
     * @param name Callback name.
     */
    [[nodiscard]] bool has_callback(const std::string &name) const;

    /**
     * @brief Call one named callback.
     *
     * @param name Callback name.
     * @param frame Current frame.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult call(
        const std::string &name,
        const Frame &frame);

    /**
     * @brief Call all registered callbacks.
     *
     * @param frame Current frame.
     */
    void update(const Frame &frame);

    /**
     * @brief Remove all callbacks.
     */
    void clear();

    /**
     * @brief Return registered callback count.
     */
    [[nodiscard]] std::size_t size() const noexcept;

    /**
     * @brief Return true if no callback is registered.
     */
    [[nodiscard]] bool empty() const noexcept;

  private:
    /**
     * @brief Attached game context.
     */
    GameContext *context_{nullptr};

    /**
     * @brief Registered script callbacks.
     */
    std::unordered_map<std::string, ScriptCallback> callbacks_{};

    /**
     * @brief Whether init() completed.
     */
    bool initialized_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_SCRIPT_RUNTIME_HPP
