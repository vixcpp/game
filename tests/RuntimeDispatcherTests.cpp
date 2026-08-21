#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>
#include <vector>

#include <vix/game/AssetManager.hpp>
#include <vix/game/AsyncAssetLoader.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/EventType.hpp>
#include <vix/game/JobSystem.hpp>
#include <vix/game/RuntimeDispatcher.hpp>

namespace
{
  std::filesystem::path dispatcher_asset_root()
  {
    const auto root = std::filesystem::temp_directory_path() / "vix_game_runtime_dispatcher_tests";
    std::filesystem::remove_all(root);
    std::filesystem::create_directories(root);
    return root;
  }

  void write_asset(const std::filesystem::path &path, const std::string &content)
  {
    std::ofstream out(path, std::ios::trunc);
    out << content;
  }
} // namespace

TEST(RuntimeDispatcherTests, ExecutesPostedTasksInFifoOrderOnBoundThread)
{
  vix::game::RuntimeDispatcher dispatcher;
  dispatcher.bind_current_thread();
  const auto runtime_thread = std::this_thread::get_id();
  std::vector<int> order;

  ASSERT_TRUE(dispatcher.post([&] { EXPECT_EQ(std::this_thread::get_id(), runtime_thread); order.push_back(1); }));
  ASSERT_TRUE(dispatcher.post([&] { EXPECT_EQ(std::this_thread::get_id(), runtime_thread); order.push_back(2); }));

  auto drained = dispatcher.drain();
  ASSERT_TRUE(drained);
  EXPECT_EQ(drained.value(), 2u);
  EXPECT_EQ(order, (std::vector<int>{1, 2}));
}

TEST(RuntimeDispatcherTests, RejectsDrainFromAnotherThread)
{
  vix::game::RuntimeDispatcher dispatcher;
  dispatcher.bind_current_thread();
  std::atomic<bool> rejected{false};

  std::thread worker([&]
  {
    auto drained = dispatcher.drain();
    rejected.store(!drained);
  });
  worker.join();

  EXPECT_TRUE(rejected.load());
}

TEST(RuntimeDispatcherTests, AsyncAssetsMutateAndCallbackOnlyOnRuntimeThread)
{
  const auto root = dispatcher_asset_root();
  write_asset(root / "one.txt", "one");
  write_asset(root / "two.txt", "two");

  vix::game::RuntimeDispatcher dispatcher;
  dispatcher.bind_current_thread();
  const auto runtime_thread = std::this_thread::get_id();
  vix::game::AssetManager assets(root.string());
  vix::game::JobSystem jobs(2);
  vix::game::AsyncAssetLoader loader(assets, jobs, dispatcher);
  std::atomic<int> callbacks{0};
  std::atomic<bool> callback_on_runtime{true};

  auto first = loader.load("one.txt", [&](vix::game::GameResult<vix::game::AssetId> result)
  {
    EXPECT_TRUE(result);
    callback_on_runtime.store(callback_on_runtime.load() && std::this_thread::get_id() == runtime_thread);
    ++callbacks;
  });
  auto second = loader.load("two.txt", [&](vix::game::GameResult<vix::game::AssetId> result)
  {
    EXPECT_TRUE(result);
    callback_on_runtime.store(callback_on_runtime.load() && std::this_thread::get_id() == runtime_thread);
    ++callbacks;
  });
  ASSERT_TRUE(first);
  ASSERT_TRUE(second);

  first.value().wait();
  second.value().wait();
  EXPECT_EQ(callbacks.load(), 0);
  EXPECT_TRUE(assets.empty());

  auto drained = dispatcher.drain();
  ASSERT_TRUE(drained);
  EXPECT_EQ(drained.value(), 2u);
  EXPECT_EQ(callbacks.load(), 2);
  EXPECT_TRUE(callback_on_runtime.load());
  EXPECT_TRUE(assets.contains("one.txt"));
  EXPECT_TRUE(assets.contains("two.txt"));

  jobs.shutdown();
}

TEST(RuntimeDispatcherTests, ClosingDispatcherDropsActiveJobCompletionWithoutDeadlock)
{
  vix::game::RuntimeDispatcher dispatcher;
  dispatcher.bind_current_thread();
  vix::game::JobSystem jobs(1);
  std::atomic<bool> started{false};
  std::atomic<bool> callback{false};

  auto job = jobs.submit([&]
  {
    started.store(true);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    (void)dispatcher.post([&] { callback.store(true); });
  });
  ASSERT_TRUE(job);
  for (int i = 0; i < 100 && !started.load(); ++i)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  ASSERT_TRUE(started.load());

  dispatcher.close();
  jobs.shutdown();

  EXPECT_EQ(dispatcher.pending(), 0u);
  EXPECT_FALSE(callback.load());
}

TEST(RuntimeDispatcherTests, JobLifecycleEventsAreDeliveredOnRuntimeThread)
{
  vix::game::RuntimeDispatcher dispatcher;
  dispatcher.bind_current_thread();
  const auto runtime_thread = std::this_thread::get_id();
  vix::game::EventBus events;
  vix::game::JobSystem jobs(1);
  jobs.set_event_bus(&events);
  jobs.set_runtime_dispatcher(&dispatcher);
  std::atomic<int> received{0};
  std::atomic<bool> correct_thread{true};

  auto subscription = events.subscribe(vix::game::EventType::JobCompleted,
      [&](const vix::game::Event &)
      {
        correct_thread.store(correct_thread.load() && std::this_thread::get_id() == runtime_thread);
        ++received;
      });
  ASSERT_TRUE(subscription);

  auto job = jobs.submit([] {});
  ASSERT_TRUE(job);
  job.value().wait();
  EXPECT_EQ(received.load(), 0);

  ASSERT_TRUE(dispatcher.drain());
  EXPECT_EQ(received.load(), 1);
  EXPECT_TRUE(correct_thread.load());
  jobs.shutdown();
}
