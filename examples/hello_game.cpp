/**
 *
 *  @file hello_game.cpp
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

int main()
{
  vix::game::App app;

  app.set_title("Hello Vix Game");
  app.set_target_fps(0);

  app.on_update(
      [&](const vix::game::Frame &frame)
      {
        vix::print("Hello from vix/game");
        vix::print("frame:", frame.index);
        vix::print("delta ms:", frame.delta_ms());

        app.stop();
      });

  auto result = app.run();

  if (!result)
  {
    vix::print("error:", result.error().message());
    return 1;
  }

  return 0;
}
