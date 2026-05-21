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

#include <vix/game/App.hpp>
#include <vix/print.hpp>

int main()
{
  vix::game::App app;

  app.set_title("Hello Vix Game");
  app.set_target_fps(60);

  vix::print("Hello from vix/game");
  vix::print("title:", app.config().title);

  return 0;
}
