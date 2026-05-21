/**
 *
 *  @file basic_loop.cpp
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

#include <cstdint>

int main()
{
  vix::game::TimeStep step;
  step.set_target_fps(0);
  step.fixed_update = false;

  vix::game::GameLoop loop(step);

  std::uint32_t frames = 0;

  loop.set_update_callback(
      [&](const vix::game::Frame &frame)
      {
        vix::print(
            "frame:",
            frame.index,
            "delta_ms:",
            frame.delta_ms(),
            "elapsed_ms:",
            frame.elapsed_ms());

        ++frames;

        if (frames >= 5)
        {
          loop.stop();
        }
      });

  auto result = loop.run();

  if (!result)
  {
    vix::print("error:", result.error().message());
    return 1;
  }

  vix::print("loop finished");
  vix::print("total frames:", loop.frame_index());

  return 0;
}
