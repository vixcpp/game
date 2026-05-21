/**
 *
 *  @file asset_loading.cpp
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

#include <filesystem>
#include <fstream>

namespace
{
  void prepare_assets()
  {
    std::filesystem::create_directories("assets");

    std::ofstream file("assets/hello.txt", std::ios::trunc);
    file << "Hello from a vix/game asset";
  }
} // namespace

int main()
{
  prepare_assets();

  vix::game::AssetManager assets("assets");

  auto asset_id = assets.load("hello.txt");
  if (!asset_id)
  {
    vix::print("error:", asset_id.error().message());
    return 1;
  }

  const auto *asset = assets.get(asset_id.value());
  if (!asset)
  {
    vix::print("error: loaded asset not found");
    return 1;
  }

  vix::print("asset loaded");
  vix::print("id:", asset->id());
  vix::print("name:", asset->name());
  vix::print("type:", vix::game::to_string(asset->type()));
  vix::print("path:", asset->path().full());
  vix::print("size:", asset->size());
  vix::print("content:", asset->text_content());

  return 0;
}
