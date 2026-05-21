/**
 *
 *  @file jobs_demo.cpp
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

#include <vix/game/game.hpp>
#include <vix/print.hpp>

#include <atomic>
#include <chrono>
#include <thread>

int main()
{
  vix::game::JobSystem jobs(2);

  auto started = jobs.start();
  if (!started)
  {
    vix::print("error:", started.error().message());
    return 1;
  }

  std::atomic<int> value{0};

  auto handle = jobs.submit(
      [&]()
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        value.store(42);
      },
      vix::game::JobPriority::High);

  if (!handle)
  {
    vix::print("error:", handle.error().message());
    return 1;
  }

  vix::print("job submitted");
  vix::print("job id:", handle.value().id());

  handle.value().wait();

  vix::print("job completed");
  vix::print("value:", value.load());
  vix::print("workers:", jobs.worker_count());

  jobs.shutdown();

  return 0;
}
