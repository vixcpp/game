#include <vix/game/RuntimeDispatcher.hpp>

#include <deque>
#include <mutex>
#include <utility>

#include <vix/game/GameError.hpp>

namespace vix::game
{
  void RuntimeDispatcher::bind_current_thread() noexcept
  {
    std::scoped_lock lock(mutex_);
    runtime_thread_ = std::this_thread::get_id();
    bound_ = true;
    closed_ = false;
  }

  bool RuntimeDispatcher::bound() const noexcept
  {
    std::scoped_lock lock(mutex_);
    return bound_;
  }

  bool RuntimeDispatcher::is_runtime_thread() const noexcept
  {
    std::scoped_lock lock(mutex_);
    return bound_ && runtime_thread_ == std::this_thread::get_id();
  }

  GameBoolResult RuntimeDispatcher::post(Task task)
  {
    if (!task)
    {
      return make_game_error(GameErrorCode::InvalidArgument, "runtime task cannot be empty");
    }

    std::scoped_lock lock(mutex_);
    if (closed_)
    {
      return make_game_error(GameErrorCode::InvalidState, "runtime dispatcher is closed");
    }

    tasks_.push_back(std::move(task));
    return true;
  }

  GameResult<std::size_t> RuntimeDispatcher::drain()
  {
    {
      std::scoped_lock lock(mutex_);
      if (!bound_)
      {
        return make_game_error(GameErrorCode::InvalidState, "runtime dispatcher is not bound to a thread");
      }
      if (runtime_thread_ != std::this_thread::get_id())
      {
        return make_game_error(GameErrorCode::InvalidState, "runtime tasks must be drained from the bound runtime thread");
      }
      if (closed_)
      {
        return std::size_t{0};
      }
    }

    std::size_t executed = 0;
    for (;;)
    {
      Task task;
      {
        std::scoped_lock lock(mutex_);
        if (tasks_.empty() || closed_)
        {
          break;
        }
        task = std::move(tasks_.front());
        tasks_.pop_front();
      }

      // A user completion must not prevent later runtime work from running.
      try
      {
        task();
      }
      catch (...)
      {
      }
      ++executed;
    }
    return executed;
  }

  void RuntimeDispatcher::close() noexcept
  {
    std::scoped_lock lock(mutex_);
    closed_ = true;
    tasks_.clear();
  }

  bool RuntimeDispatcher::closed() const noexcept
  {
    std::scoped_lock lock(mutex_);
    return closed_;
  }

  std::size_t RuntimeDispatcher::pending() const noexcept
  {
    std::scoped_lock lock(mutex_);
    return tasks_.size();
  }
} // namespace vix::game
