/**
 *
 *  @file JobSystemTests.cpp
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

#include <atomic>
#include <chrono>
#include <thread>

#include <vix/game/EventBus.hpp>
#include <vix/game/EventType.hpp>
#include <vix/game/JobHandle.hpp>
#include <vix/game/JobPriority.hpp>
#include <vix/game/JobSystem.hpp>

TEST(JobSystemTests, DefaultJobSystemCanStartAndShutdown)
{
  vix::game::JobSystem jobs;

  auto started = jobs.start();

  ASSERT_TRUE(started);
  EXPECT_TRUE(started.value());
  EXPECT_TRUE(jobs.running());
  EXPECT_GE(jobs.worker_count(), 1u);

  jobs.shutdown();

  EXPECT_FALSE(jobs.running());
}

TEST(JobSystemTests, ExplicitWorkerCountIsStored)
{
  vix::game::JobSystem jobs(2);

  EXPECT_EQ(jobs.worker_count(), 2u);

  auto started = jobs.start();

  ASSERT_TRUE(started);
  EXPECT_TRUE(jobs.running());

  jobs.shutdown();
}

TEST(JobSystemTests, ZeroWorkerCountBecomesOne)
{
  vix::game::JobSystem jobs(0);

  EXPECT_EQ(jobs.worker_count(), 1u);
}

TEST(JobSystemTests, SubmitRunsJob)
{
  vix::game::JobSystem jobs(1);

  std::atomic<int> value{0};

  auto handle = jobs.submit(
      [&]()
      {
        value.store(42);
      });

  ASSERT_TRUE(handle);
  EXPECT_TRUE(handle.value().valid());

  handle.value().wait();

  EXPECT_EQ(value.load(), 42);

  jobs.shutdown();
}

TEST(JobSystemTests, SubmitRejectsEmptyJob)
{
  vix::game::JobSystem jobs(1);

  auto handle = jobs.submit(vix::game::JobSystem::Job{});

  EXPECT_FALSE(handle);
}

TEST(JobSystemTests, SubmitDetachedRunsJob)
{
  vix::game::JobSystem jobs(1);

  std::atomic<int> value{0};

  auto result = jobs.submit_detached(
      [&]()
      {
        value.store(7);
      });

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());

  for (int i = 0; i < 100 && value.load() != 7; ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  EXPECT_EQ(value.load(), 7);

  jobs.shutdown();
}

TEST(JobSystemTests, SubmitDetachedRejectsEmptyJob)
{
  vix::game::JobSystem jobs(1);

  auto result = jobs.submit_detached(vix::game::JobSystem::Job{});

  EXPECT_FALSE(result);
}

TEST(JobSystemTests, JobHandleReportsValidJobId)
{
  vix::game::JobSystem jobs(1);

  auto handle = jobs.submit(
      []() {});

  ASSERT_TRUE(handle);

  const auto id = handle.value().id();

  EXPECT_TRUE(vix::game::is_valid_job_id(id));
  EXPECT_NE(id, vix::game::invalid_job_id);

  handle.value().wait();

  jobs.shutdown();
}

TEST(JobSystemTests, JobPriorityMapsToThreadPoolPriority)
{
  EXPECT_EQ(
      vix::game::to_task_priority(vix::game::JobPriority::Lowest),
      vix::threadpool::TaskPriority::lowest);

  EXPECT_EQ(
      vix::game::to_task_priority(vix::game::JobPriority::Low),
      vix::threadpool::TaskPriority::low);

  EXPECT_EQ(
      vix::game::to_task_priority(vix::game::JobPriority::Normal),
      vix::threadpool::TaskPriority::normal);

  EXPECT_EQ(
      vix::game::to_task_priority(vix::game::JobPriority::High),
      vix::threadpool::TaskPriority::high);

  EXPECT_EQ(
      vix::game::to_task_priority(vix::game::JobPriority::Highest),
      vix::threadpool::TaskPriority::highest);
}

TEST(JobSystemTests, DispatchesJobStartedEvent)
{
  vix::game::EventBus bus;
  vix::game::JobSystem jobs(1);

  jobs.set_event_bus(&bus);

  std::atomic<int> started_events{0};

  auto listener = bus.subscribe(
      vix::game::EventType::JobStarted,
      [&](const vix::game::Event &event)
      {
        EXPECT_EQ(event.source, "job_system");
        EXPECT_TRUE(event.has_field("job_id"));
        started_events.fetch_add(1);
      });

  ASSERT_TRUE(listener);

  auto handle = jobs.submit(
      []() {});

  ASSERT_TRUE(handle);

  handle.value().wait();

  EXPECT_EQ(started_events.load(), 1);

  jobs.shutdown();
}

TEST(JobSystemTests, StatsCanBeQueried)
{
  vix::game::JobSystem jobs(1);

  auto started = jobs.start();

  ASSERT_TRUE(started);

  auto stats = jobs.stats();

  (void)stats;

  jobs.shutdown();
}
