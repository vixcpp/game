/**
 *
 *  @file EventBusTests.cpp
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

#include <string>

#include <vix/game/Event.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/EventType.hpp>

TEST(EventBusTests, SubscribeAndDispatchTypedEvent)
{
  vix::game::EventBus bus;

  int calls = 0;

  auto listener = bus.subscribe(
      vix::game::EventType::AppStarted,
      [&](const vix::game::Event &event)
      {
        ++calls;
        EXPECT_EQ(event.type, vix::game::EventType::AppStarted);
        EXPECT_EQ(event.event_name(), "app_started");
      });

  ASSERT_TRUE(listener);
  EXPECT_TRUE(vix::game::is_valid_event_listener_id(listener.value()));

  auto result = bus.dispatch(vix::game::Event(vix::game::EventType::AppStarted));

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_EQ(calls, 1);
  EXPECT_EQ(bus.dispatched_count(), 1u);
}

TEST(EventBusTests, SubscribeAndDispatchCustomNamedEvent)
{
  vix::game::EventBus bus;

  int calls = 0;

  auto listener = bus.subscribe(
      "player_spawned",
      [&](const vix::game::Event &event)
      {
        ++calls;
        EXPECT_TRUE(event.is_custom());
        EXPECT_EQ(event.event_name(), "player_spawned");
      });

  ASSERT_TRUE(listener);

  auto result = bus.dispatch(vix::game::Event::custom("player_spawned"));

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_EQ(calls, 1);
}

TEST(EventBusTests, SubscribeAllReceivesEveryEvent)
{
  vix::game::EventBus bus;

  int calls = 0;

  auto listener = bus.subscribe_all(
      [&](const vix::game::Event &event)
      {
        (void)event;
        ++calls;
      });

  ASSERT_TRUE(listener);

  auto first = bus.dispatch(vix::game::Event(vix::game::EventType::AppStarted));
  auto second = bus.dispatch(vix::game::Event(vix::game::EventType::FrameStarted));

  ASSERT_TRUE(first);
  ASSERT_TRUE(second);

  EXPECT_EQ(calls, 2);
  EXPECT_EQ(bus.dispatched_count(), 2u);
}

TEST(EventBusTests, UnsubscribeRemovesListener)
{
  vix::game::EventBus bus;

  int calls = 0;

  auto listener = bus.subscribe(
      vix::game::EventType::AppStarted,
      [&](const vix::game::Event &event)
      {
        (void)event;
        ++calls;
      });

  ASSERT_TRUE(listener);

  EXPECT_TRUE(bus.unsubscribe(listener.value()));

  auto result = bus.dispatch(vix::game::Event(vix::game::EventType::AppStarted));

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_EQ(calls, 0);
}

TEST(EventBusTests, EmptyListenerIsRejected)
{
  vix::game::EventBus bus;

  auto listener = bus.subscribe(
      vix::game::EventType::AppStarted,
      vix::game::EventBus::Listener{});

  EXPECT_FALSE(listener);
}

TEST(EventBusTests, EmptyCustomEventNameIsRejected)
{
  vix::game::EventBus bus;

  auto listener = bus.subscribe(
      "",
      [](const vix::game::Event &event)
      {
        (void)event;
      });

  EXPECT_FALSE(listener);
}

TEST(EventBusTests, ListenerCountTracksRegisteredListeners)
{
  vix::game::EventBus bus;

  auto a = bus.subscribe(
      vix::game::EventType::AppStarted,
      [](const vix::game::Event &event)
      {
        (void)event;
      });

  auto b = bus.subscribe_all(
      [](const vix::game::Event &event)
      {
        (void)event;
      });

  auto c = bus.subscribe(
      "custom_event",
      [](const vix::game::Event &event)
      {
        (void)event;
      });

  ASSERT_TRUE(a);
  ASSERT_TRUE(b);
  ASSERT_TRUE(c);

  EXPECT_EQ(bus.listener_count(), 3u);

  bus.clear();

  EXPECT_EQ(bus.listener_count(), 0u);
}

TEST(EventBusTests, EventFieldsArePreservedDuringDispatch)
{
  vix::game::EventBus bus;

  std::string value;

  auto listener = bus.subscribe(
      vix::game::EventType::SceneLoaded,
      [&](const vix::game::Event &event)
      {
        value = event.field_or("scene_name");
      });

  ASSERT_TRUE(listener);

  vix::game::Event event(vix::game::EventType::SceneLoaded);
  event.set_field("scene_name", "main");

  auto result = bus.dispatch(std::move(event));

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_EQ(value, "main");
}

TEST(EventBusTests, DispatchAssignsSequence)
{
  vix::game::EventBus bus;

  std::uint64_t sequence = 0;

  auto listener = bus.subscribe_all(
      [&](const vix::game::Event &event)
      {
        sequence = event.sequence;
      });

  ASSERT_TRUE(listener);

  auto result = bus.dispatch(vix::game::Event(vix::game::EventType::FrameStarted));

  ASSERT_TRUE(result);
  EXPECT_TRUE(result.value());
  EXPECT_EQ(sequence, 1u);
}
