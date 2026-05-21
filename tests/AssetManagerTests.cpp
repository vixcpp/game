/**
 *
 *  @file AssetManagerTests.cpp
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

#include <filesystem>
#include <fstream>
#include <string>

#include <vix/game/Asset.hpp>
#include <vix/game/AssetId.hpp>
#include <vix/game/AssetManager.hpp>
#include <vix/game/AssetType.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/EventType.hpp>

namespace
{
  std::filesystem::path make_test_asset_root()
  {
    auto root = std::filesystem::temp_directory_path() / "vix_game_asset_tests";
    std::filesystem::remove_all(root);
    std::filesystem::create_directories(root);
    return root;
  }

  void write_text_file(const std::filesystem::path &path, const std::string &content)
  {
    std::filesystem::create_directories(path.parent_path());

    std::ofstream out(path, std::ios::trunc);
    out << content;
  }

  void write_binary_file(const std::filesystem::path &path)
  {
    std::filesystem::create_directories(path.parent_path());

    std::ofstream out(path, std::ios::binary | std::ios::trunc);

    const unsigned char bytes[] = {0x01, 0x02, 0x03, 0x04};
    out.write(reinterpret_cast<const char *>(bytes), sizeof(bytes));
  }
} // namespace

TEST(AssetManagerTests, LoadTextAsset)
{
  const auto root = make_test_asset_root();
  write_text_file(root / "hello.txt", "hello vix");

  vix::game::AssetManager manager(root.string());

  auto id = manager.load("hello.txt");

  ASSERT_TRUE(id);
  EXPECT_TRUE(vix::game::is_valid_asset_id(id.value()));
  EXPECT_TRUE(manager.contains(id.value()));
  EXPECT_TRUE(manager.contains("hello.txt"));
  EXPECT_EQ(manager.size(), 1u);

  const auto *asset = manager.get(id.value());

  ASSERT_NE(asset, nullptr);
  EXPECT_TRUE(asset->loaded());
  EXPECT_TRUE(asset->textual());
  EXPECT_FALSE(asset->binary());
  EXPECT_EQ(asset->type(), vix::game::AssetType::Text);
  EXPECT_EQ(asset->text_content(), "hello vix");
  EXPECT_EQ(asset->path().relative(), "hello.txt");
}

TEST(AssetManagerTests, LoadJsonAsset)
{
  const auto root = make_test_asset_root();
  write_text_file(root / "config.json", R"({"name":"vix"})");

  vix::game::AssetManager manager(root.string());

  auto id = manager.load("config.json");

  ASSERT_TRUE(id);

  const auto *asset = manager.get(id.value());

  ASSERT_NE(asset, nullptr);
  EXPECT_EQ(asset->type(), vix::game::AssetType::Json);
  EXPECT_TRUE(asset->textual());
  EXPECT_EQ(asset->text_content(), R"({"name":"vix"})");
}

TEST(AssetManagerTests, LoadBinaryAsset)
{
  const auto root = make_test_asset_root();
  write_binary_file(root / "data.bin");

  vix::game::AssetManager manager(root.string());

  auto id = manager.load("data.bin");

  ASSERT_TRUE(id);

  const auto *asset = manager.get(id.value());

  ASSERT_NE(asset, nullptr);
  EXPECT_TRUE(asset->loaded());
  EXPECT_TRUE(asset->binary());
  EXPECT_FALSE(asset->textual());
  EXPECT_EQ(asset->type(), vix::game::AssetType::Binary);
  EXPECT_EQ(asset->bytes().size(), 4u);
}

TEST(AssetManagerTests, LoadMissingAssetReturnsError)
{
  const auto root = make_test_asset_root();

  vix::game::AssetManager manager(root.string());

  auto id = manager.load("missing.txt");

  EXPECT_FALSE(id);
  EXPECT_TRUE(manager.empty());
}

TEST(AssetManagerTests, LoadDuplicateAssetReturnsError)
{
  const auto root = make_test_asset_root();
  write_text_file(root / "hello.txt", "hello");

  vix::game::AssetManager manager(root.string());

  auto first = manager.load("hello.txt");
  ASSERT_TRUE(first);

  auto second = manager.load("hello.txt");

  EXPECT_FALSE(second);
  EXPECT_EQ(manager.size(), 1u);
}

TEST(AssetManagerTests, UnloadAssetById)
{
  const auto root = make_test_asset_root();
  write_text_file(root / "hello.txt", "hello");

  vix::game::AssetManager manager(root.string());

  auto id = manager.load("hello.txt");
  ASSERT_TRUE(id);

  auto unloaded = manager.unload(id.value());

  ASSERT_TRUE(unloaded);
  EXPECT_TRUE(unloaded.value());
  EXPECT_FALSE(manager.contains(id.value()));
  EXPECT_FALSE(manager.contains("hello.txt"));
  EXPECT_TRUE(manager.empty());
}

TEST(AssetManagerTests, UnloadAssetByPath)
{
  const auto root = make_test_asset_root();
  write_text_file(root / "hello.txt", "hello");

  vix::game::AssetManager manager(root.string());

  auto id = manager.load("hello.txt");
  ASSERT_TRUE(id);

  auto unloaded = manager.unload("hello.txt");

  ASSERT_TRUE(unloaded);
  EXPECT_TRUE(unloaded.value());
  EXPECT_FALSE(manager.contains(id.value()));
  EXPECT_FALSE(manager.contains("hello.txt"));
}

TEST(AssetManagerTests, GetByRelativePath)
{
  const auto root = make_test_asset_root();
  write_text_file(root / "hello.txt", "hello");

  vix::game::AssetManager manager(root.string());

  auto id = manager.load("hello.txt");
  ASSERT_TRUE(id);

  auto *asset = manager.get("hello.txt");

  ASSERT_NE(asset, nullptr);
  EXPECT_EQ(asset->id(), id.value());
  EXPECT_EQ(manager.id_for("hello.txt"), id.value());
}

TEST(AssetManagerTests, IdForMissingPathReturnsInvalidId)
{
  const auto root = make_test_asset_root();

  vix::game::AssetManager manager(root.string());

  EXPECT_EQ(manager.id_for("missing.txt"), vix::game::invalid_asset_id);
}

TEST(AssetManagerTests, ClearRemovesAllAssets)
{
  const auto root = make_test_asset_root();
  write_text_file(root / "a.txt", "a");
  write_text_file(root / "b.txt", "b");

  vix::game::AssetManager manager(root.string());

  ASSERT_TRUE(manager.load("a.txt"));
  ASSERT_TRUE(manager.load("b.txt"));

  EXPECT_EQ(manager.size(), 2u);

  manager.clear();

  EXPECT_TRUE(manager.empty());
  EXPECT_EQ(manager.size(), 0u);
  EXPECT_EQ(manager.id_for("a.txt"), vix::game::invalid_asset_id);
}

TEST(AssetManagerTests, DispatchesAssetLoadedEvent)
{
  const auto root = make_test_asset_root();
  write_text_file(root / "hello.txt", "hello");

  vix::game::EventBus bus;
  vix::game::AssetManager manager(root.string());

  manager.set_event_bus(&bus);

  int loaded_events = 0;

  auto listener = bus.subscribe(
      vix::game::EventType::AssetLoaded,
      [&](const vix::game::Event &event)
      {
        ++loaded_events;
        EXPECT_EQ(event.source, "asset_manager");
        EXPECT_EQ(event.target, "hello.txt");
        EXPECT_EQ(event.field_or("path"), "hello.txt");
      });

  ASSERT_TRUE(listener);

  auto id = manager.load("hello.txt");

  ASSERT_TRUE(id);
  EXPECT_EQ(loaded_events, 1);
}
