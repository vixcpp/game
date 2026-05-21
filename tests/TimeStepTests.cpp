/**
 *
 *  @file TimeStepTests.cpp
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

#include <vix/game/TimeStep.hpp>
#include <vix/time/Duration.hpp>

TEST(TimeStepTests, DefaultValuesAreValid)
{
  vix::game::TimeStep step;

  EXPECT_EQ(step.target_fps, 60u);
  EXPECT_TRUE(step.limit_fps);
  EXPECT_TRUE(step.fixed_update);
  EXPECT_EQ(step.fixed_delta.count_ms(), 16);
  EXPECT_EQ(step.max_frame_delta.count_ms(), 250);
}

TEST(TimeStepTests, FromFpsConfiguresTargetFps)
{
  const auto step = vix::game::TimeStep::from_fps(30);

  EXPECT_EQ(step.target_fps, 30u);
  EXPECT_TRUE(step.limit_fps);
}

TEST(TimeStepTests, SetTargetFpsZeroDisablesFpsLimit)
{
  vix::game::TimeStep step;

  step.set_target_fps(0);

  EXPECT_EQ(step.target_fps, 0u);
  EXPECT_FALSE(step.limit_fps);
  EXPECT_TRUE(step.target_frame_duration().is_zero());
}

TEST(TimeStepTests, TargetFrameDurationUsesNanoseconds)
{
  vix::game::TimeStep step;

  step.set_target_fps(60);

  const auto duration = step.target_frame_duration();

  EXPECT_GT(duration.count_ns(), 0);
  EXPECT_EQ(duration.count_ms(), 16);
}

TEST(TimeStepTests, ClampDeltaReturnsSameValueWhenBelowMaximum)
{
  vix::game::TimeStep step;
  step.set_max_frame_delta(vix::time::Duration::milliseconds(250));

  const auto delta = vix::time::Duration::milliseconds(12);
  const auto clamped = step.clamp_delta(delta);

  EXPECT_EQ(clamped, delta);
}

TEST(TimeStepTests, ClampDeltaLimitsLargeValue)
{
  vix::game::TimeStep step;
  step.set_max_frame_delta(vix::time::Duration::milliseconds(250));

  const auto delta = vix::time::Duration::seconds(3);
  const auto clamped = step.clamp_delta(delta);

  EXPECT_EQ(clamped, vix::time::Duration::milliseconds(250));
}

TEST(TimeStepTests, ZeroMaxFrameDeltaDisablesClamp)
{
  vix::game::TimeStep step;
  step.set_max_frame_delta(vix::time::Duration{});

  const auto delta = vix::time::Duration::seconds(3);
  const auto clamped = step.clamp_delta(delta);

  EXPECT_EQ(clamped, delta);
}

TEST(TimeStepTests, SetFixedDeltaUpdatesValue)
{
  vix::game::TimeStep step;

  step.set_fixed_delta(vix::time::Duration::milliseconds(8));

  EXPECT_EQ(step.fixed_delta.count_ms(), 8);
}

TEST(TimeStepTests, SetMaxFrameDeltaUpdatesValue)
{
  vix::game::TimeStep step;

  step.set_max_frame_delta(vix::time::Duration::milliseconds(100));

  EXPECT_EQ(step.max_frame_delta.count_ms(), 100);
}
