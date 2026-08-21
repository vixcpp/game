/**
 * @file RuntimeDispatcher.hpp
 * @brief FIFO hand-off from worker threads to the runtime thread.
 */
#ifndef VIX_GAME_RUNTIME_DISPATCHER_HPP
#define VIX_GAME_RUNTIME_DISPATCHER_HPP

#include <cstddef>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>

#include <vix/game/GameResult.hpp>

namespace vix::game
{
  /**
   * @brief Thread-safe queue whose operations execute only on its bound runtime
   * thread.
   *
   * Posting is safe from any thread.  Draining must be performed by the thread
   * that called bind_current_thread(); tasks are executed FIFO.  The dispatcher
   * deliberately does not make the services used by a task thread-safe.
   */
  class RuntimeDispatcher
  {
  public:
    using Task = std::function<void()>;

    RuntimeDispatcher() = default;
    RuntimeDispatcher(const RuntimeDispatcher &) = delete;
    RuntimeDispatcher &operator=(const RuntimeDispatcher &) = delete;

    void bind_current_thread() noexcept;
    [[nodiscard]] bool bound() const noexcept;
    [[nodiscard]] bool is_runtime_thread() const noexcept;

    [[nodiscard]] GameBoolResult post(Task task);
    [[nodiscard]] GameResult<std::size_t> drain();

    /** Reject future tasks and discard tasks which have not started. */
    void close() noexcept;
    [[nodiscard]] bool closed() const noexcept;
    [[nodiscard]] std::size_t pending() const noexcept;

  private:
    mutable std::mutex mutex_{};
    std::deque<Task> tasks_{};
    std::thread::id runtime_thread_{};
    bool bound_{false};
    bool closed_{false};
  };
} // namespace vix::game

#endif // VIX_GAME_RUNTIME_DISPATCHER_HPP
