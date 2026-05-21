/**
 *
 *  @file JobPriority.hpp
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
#ifndef VIX_GAME_JOB_PRIORITY_HPP
#define VIX_GAME_JOB_PRIORITY_HPP

#include <cstdint>

#include <vix/threadpool/TaskPriority.hpp>

namespace vix::game
{
  /**
   * @brief Priority level for jobs submitted through JobSystem.
   *
   * JobPriority is the game-facing priority model. It maps directly to
   * vix::threadpool::TaskPriority internally, but keeps the public game API
   * independent from threadpool naming.
   */
  enum class JobPriority : std::int32_t
  {
    /**
     * @brief Lowest priority job.
     */
    Lowest = -2,

    /**
     * @brief Low priority job.
     */
    Low = -1,

    /**
     * @brief Normal priority job.
     */
    Normal = 0,

    /**
     * @brief High priority job.
     */
    High = 1,

    /**
     * @brief Highest priority job.
     */
    Highest = 2
  };

  /**
   * @brief Convert a job priority to its numeric value.
   *
   * @param priority Job priority.
   * @return Numeric priority value.
   */
  [[nodiscard]] inline constexpr std::int32_t to_priority_value(
      JobPriority priority) noexcept
  {
    return static_cast<std::int32_t>(priority);
  }

  /**
   * @brief Convert a job priority to a threadpool task priority.
   *
   * @param priority Job priority.
   * @return Matching threadpool task priority.
   */
  [[nodiscard]] inline constexpr vix::threadpool::TaskPriority to_task_priority(
      JobPriority priority) noexcept
  {
    using vix::threadpool::TaskPriority;

    switch (priority)
    {
    case JobPriority::Lowest:
      return TaskPriority::lowest;
    case JobPriority::Low:
      return TaskPriority::low;
    case JobPriority::Normal:
      return TaskPriority::normal;
    case JobPriority::High:
      return TaskPriority::high;
    case JobPriority::Highest:
      return TaskPriority::highest;
    }

    return TaskPriority::normal;
  }

  /**
   * @brief Convert a threadpool task priority to a game job priority.
   *
   * @param priority Threadpool task priority.
   * @return Matching game job priority.
   */
  [[nodiscard]] inline constexpr JobPriority from_task_priority(
      vix::threadpool::TaskPriority priority) noexcept
  {
    using vix::threadpool::TaskPriority;

    switch (priority)
    {
    case TaskPriority::lowest:
      return JobPriority::Lowest;
    case TaskPriority::low:
      return JobPriority::Low;
    case TaskPriority::normal:
      return JobPriority::Normal;
    case TaskPriority::high:
      return JobPriority::High;
    case TaskPriority::highest:
      return JobPriority::Highest;
    }

    return JobPriority::Normal;
  }

  /**
   * @brief Convert a job priority to a stable readable name.
   *
   * @param priority Job priority.
   * @return Static priority name.
   */
  [[nodiscard]] inline constexpr const char *to_string(
      JobPriority priority) noexcept
  {
    switch (priority)
    {
    case JobPriority::Lowest:
      return "lowest";
    case JobPriority::Low:
      return "low";
    case JobPriority::Normal:
      return "normal";
    case JobPriority::High:
      return "high";
    case JobPriority::Highest:
      return "highest";
    }

    return "unknown";
  }

  /**
   * @brief Check whether lhs has higher priority than rhs.
   *
   * @param lhs Left priority.
   * @param rhs Right priority.
   * @return true if lhs is higher than rhs.
   */
  [[nodiscard]] inline constexpr bool priority_higher_than(
      JobPriority lhs,
      JobPriority rhs) noexcept
  {
    return to_priority_value(lhs) > to_priority_value(rhs);
  }

} // namespace vix::game

#endif // VIX_GAME_JOB_PRIORITY_HPP
