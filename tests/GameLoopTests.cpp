/**
 *
 *  @file GameLoopTests.cpp
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

#include <gtest/gtest.h>

#include <cstdint>

#include <vix/game/GameLoop.hpp>
#include <vix/game/TimeStep.hpp>
#include <vix/time/Duration.hpp>

TEST(GameLoopTests, LoopStartsAndStopsFromUpdateCallback)
{
  vix::game::TimeStep step;
  step.set_target_fps(0);
  step.fixed_update = false;

  vix::game::GameLoop loop(step);

  std::uint32_t updates = 0;

  loop.set_update_callback(
      [&](const vix::game::Frame &frame)
      {
        EXPECT_EQ(frame.index, 0u);

        ++updates;
        loop.stop();
      });

  auto result = loop.run();

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_FALSE(loop.running());
  EXPECT_EQ(updates, 1u);
  EXPECT_EQ(loop.frame_index(), 1u);
}

TEST(GameLoopTests, LoopRejectsNestedRun)
{
  vix::game::TimeStep step;
  step.set_target_fps(0);
  step.fixed_update = false;

  vix::game::GameLoop loop(step);

  bool nested_failed = false;

  loop.set_update_callback(
      [&](const vix::game::Frame &frame)
      {
        (void)frame;

        auto nested = loop.run();
        nested_failed = !nested;

        loop.stop();
      });

  auto result = loop.run();

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_TRUE(nested_failed);
}

TEST(GameLoopTests, ShouldContinueCallbackCanStopLoop)
{
  vix::game::TimeStep step;
  step.set_target_fps(0);
  step.fixed_update = false;

  vix::game::GameLoop loop(step);

  std::uint32_t updates = 0;

  loop.set_update_callback(
      [&](const vix::game::Frame &frame)
      {
        (void)frame;
        ++updates;
      });

  loop.set_should_continue_callback(
      [&]()
      {
        return updates < 3;
      });

  auto result = loop.run();

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_EQ(updates, 3u);
  EXPECT_EQ(loop.frame_index(), 3u);
}

TEST(GameLoopTests, FixedUpdateRunsWhenEnabled)
{
  vix::game::TimeStep step;
  step.set_target_fps(0);
  step.fixed_update = true;
  step.fixed_delta = vix::time::Duration::milliseconds(1);
  step.max_frame_delta = vix::time::Duration::milliseconds(16);

  vix::game::GameLoop loop(step);

  std::uint32_t fixed_updates = 0;
  std::uint32_t updates = 0;

  loop.set_fixed_update_callback(
      [&](const vix::game::Frame &frame)
      {
        (void)frame;
        ++fixed_updates;
      });

  loop.set_update_callback(
      [&](const vix::game::Frame &frame)
      {
        (void)frame;

        ++updates;

        if (updates >= 5)
        {
          loop.stop();
        }
      });

  auto result = loop.run();

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_GE(updates, 1u);
}

TEST(GameLoopTests, LastFrameIsUpdated)
{
  vix::game::TimeStep step;
  step.set_target_fps(0);
  step.fixed_update = false;

  vix::game::GameLoop loop(step);

  loop.set_update_callback(
      [&](const vix::game::Frame &frame)
      {
        (void)frame;
        loop.stop();
      });

  auto result = loop.run();

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());

  const auto &last = loop.last_frame();
  EXPECT_EQ(last.index, 0u);
  EXPECT_TRUE(last.first());
}
