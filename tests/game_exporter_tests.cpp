/**
 *
 *  @file game_exporter_tests.cpp
 *  @author Gaspard Kirira
 *
 *  Vix.cpp
 *
 */

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string>

#include <vix/game/GameExportConfig.hpp>
#include <vix/game/GameExporter.hpp>
#include <vix/game/GamePackage.hpp>
#include <vix/game/GameAssetPipeline.hpp>
#include <vix/game/GameExportAsset.hpp>
#include <vix/json/loads.hpp>
#include <sstream>

namespace
{
  std::filesystem::path make_export_test_root(const std::string &name)
  {
    const auto root =
        std::filesystem::temp_directory_path() / "vix_game_exporter_tests" / name;

    std::error_code ec;
    std::filesystem::remove_all(root, ec);
    std::filesystem::create_directories(root);

    return root;
  }

  void write_text_file(
      const std::filesystem::path &path,
      const std::string &content)
  {
    std::filesystem::create_directories(path.parent_path());

    std::ofstream file(path);
    file << content;
  }

  bool file_exists(const std::filesystem::path &path)
  {
    return std::filesystem::exists(path) &&
           std::filesystem::is_regular_file(path);
  }

  bool directory_exists(const std::filesystem::path &path)
  {
    return std::filesystem::exists(path) &&
           std::filesystem::is_directory(path);
  }

  vix::json::Json read_json_file(const std::filesystem::path &path)
  {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    auto json = vix::json::try_loads(buffer.str());
    EXPECT_TRUE(json);

    if (!json)
    {
      return vix::json::Json::object();
    }

    return json.value();
  }
}

TEST(GameExporterTests, ExportsValidProject)
{
  const auto root = make_export_test_root("valid_project");

  write_text_file(
      root / "game.package.json",
      R"({"name":"demo-game","version":"0.1.0"})");

  vix::game::GameExportConfig config;
  config.project_root = root;
  config.output_directory = root / "dist";
  config.name = "demo-game";

  vix::game::GameExporter exporter;

  auto result = exporter.export_project(config);

  ASSERT_TRUE(result);
  EXPECT_EQ(result.value().output_path, root / "dist" / "demo-game");
  EXPECT_EQ(result.value().copied_files, 2U);
  EXPECT_TRUE(file_exists(root / "dist" / "demo-game" / "game.package.json"));
}

TEST(GameExporterTests, FailsWithInvalidProjectRoot)
{
  const auto root = make_export_test_root("invalid_project_root");

  vix::game::GameExportConfig config;
  config.project_root = root / "missing";
  config.output_directory = root / "dist";
  config.name = "demo-game";

  vix::game::GameExporter exporter;

  auto result = exporter.export_project(config);

  ASSERT_FALSE(result);
}

TEST(GameExporterTests, FailsWhenOutputExistsAndOverwriteIsFalse)
{
  const auto root = make_export_test_root("overwrite_false");

  write_text_file(
      root / "game.package.json",
      R"({"name":"demo-game","version":"0.1.0"})");

  std::filesystem::create_directories(root / "dist" / "demo-game");

  vix::game::GameExportConfig config;
  config.project_root = root;
  config.output_directory = root / "dist";
  config.name = "demo-game";
  config.overwrite = false;

  vix::game::GameExporter exporter;

  auto result = exporter.export_project(config);

  ASSERT_FALSE(result);
}

TEST(GameExporterTests, ExportsProjectWithAssets)
{
  const auto root = make_export_test_root("with_assets");

  write_text_file(
      root / "game.package.json",
      R"({"name":"demo-game","version":"0.1.0"})");

  write_text_file(
      root / "assets" / "player.txt",
      "player asset");

  write_text_file(
      root / "assets" / "levels" / "level1.txt",
      "level data");

  vix::game::GameExportConfig config;
  config.project_root = root;
  config.output_directory = root / "dist";
  config.name = "demo-game";

  vix::game::GameExporter exporter;

  auto result = exporter.export_project(config);

  ASSERT_TRUE(result);
  EXPECT_EQ(result.value().copied_files, 4U);
  EXPECT_GE(result.value().copied_directories, 2U);

  EXPECT_TRUE(file_exists(root / "dist" / "demo-game" / "game.package.json"));
  EXPECT_TRUE(file_exists(root / "dist" / "demo-game" / "assets" / "player.txt"));
  EXPECT_TRUE(file_exists(root / "dist" / "demo-game" / "assets" / "levels" / "level1.txt"));
}

TEST(GameExporterTests, ExportsProjectWithoutAssets)
{
  const auto root = make_export_test_root("without_assets");

  write_text_file(
      root / "game.package.json",
      R"({"name":"demo-game","version":"0.1.0"})");

  vix::game::GameExportConfig config;
  config.project_root = root;
  config.output_directory = root / "dist";
  config.name = "demo-game";

  vix::game::GameExporter exporter;

  auto result = exporter.export_project(config);

  ASSERT_TRUE(result);
  EXPECT_EQ(result.value().copied_files, 2U);
  EXPECT_EQ(result.value().copied_directories, 0U);

  EXPECT_TRUE(file_exists(root / "dist" / "demo-game" / "game.package.json"));
  EXPECT_FALSE(directory_exists(root / "dist" / "demo-game" / "assets"));
}

TEST(GameExporterTests, ExportsProjectWithoutReadme)
{
  const auto root = make_export_test_root("without_readme");

  write_text_file(
      root / "game.package.json",
      R"({"name":"demo-game","version":"0.1.0"})");

  vix::game::GameExportConfig config;
  config.project_root = root;
  config.output_directory = root / "dist";
  config.name = "demo-game";
  config.copy_readme = true;

  vix::game::GameExporter exporter;

  auto result = exporter.export_project(config);

  ASSERT_TRUE(result);
  EXPECT_EQ(result.value().copied_files, 2U);

  EXPECT_TRUE(file_exists(root / "dist" / "demo-game" / "game.package.json"));
  EXPECT_FALSE(file_exists(root / "dist" / "demo-game" / "README.md"));
}

TEST(GamePackageTests, SavesPackageFile)
{
  const auto root = make_export_test_root("package_save_file");

  vix::game::GamePackage package;
  package.name = "demo-game";
  package.version = "0.1.0";
  package.entry_scene = "main";
  package.asset_root = "assets";
  package.output_dir = "dist";
  package.scenes = {"main"};
  package.assets = {"assets/player.png"};

  const auto path = root / "game.package.json";

  auto saved = package.save_file(path.string());

  ASSERT_TRUE(saved);
  EXPECT_TRUE(file_exists(path));
}

TEST(GamePackageTests, LoadsPackageFile)
{
  const auto root = make_export_test_root("package_load_file");

  write_text_file(
      root / "game.package.json",
      R"({
        "name": "loaded-game",
        "version": "1.2.3",
        "author": "Gaspard Kirira",
        "entry_scene": "main",
        "asset_root": "assets",
        "output_dir": "release",
        "scenes": ["main", "menu"],
        "assets": ["assets/player.png", "assets/level1.json"]
      })");

  auto package = vix::game::GamePackage::load_file(
      (root / "game.package.json").string());

  ASSERT_TRUE(package);
  EXPECT_EQ(package.value().name, "loaded-game");
  EXPECT_EQ(package.value().version, "1.2.3");
  EXPECT_EQ(package.value().author, "Gaspard Kirira");
  EXPECT_EQ(package.value().entry_scene, "main");
  EXPECT_EQ(package.value().asset_root, "assets");
  EXPECT_EQ(package.value().output_dir, "release");

  ASSERT_EQ(package.value().scenes.size(), 2U);
  EXPECT_EQ(package.value().scenes[0], "main");
  EXPECT_EQ(package.value().scenes[1], "menu");

  ASSERT_EQ(package.value().assets.size(), 2U);
  EXPECT_EQ(package.value().assets[0], "assets/player.png");
  EXPECT_EQ(package.value().assets[1], "assets/level1.json");
}

TEST(GamePackageTests, FailsValidationWhenNameIsEmpty)
{
  auto package = vix::game::GamePackage::defaults();
  package.name.clear();

  auto valid = package.validate();

  ASSERT_FALSE(valid);
}

TEST(GamePackageTests, FailsValidationWhenVersionIsEmpty)
{
  auto package = vix::game::GamePackage::defaults();
  package.version.clear();

  auto valid = package.validate();

  ASSERT_FALSE(valid);
}

TEST(GamePackageTests, FailsValidationWhenEntrySceneIsEmpty)
{
  auto package = vix::game::GamePackage::defaults();
  package.entry_scene.clear();

  auto valid = package.validate();

  ASSERT_FALSE(valid);
}

TEST(GamePackageTests, FailsValidationWhenAssetRootIsEmpty)
{
  auto package = vix::game::GamePackage::defaults();
  package.asset_root.clear();

  auto valid = package.validate();

  ASSERT_FALSE(valid);
}

TEST(GameExporterTests, UsesGamePackageNameOutputDirectoryAndAssetRoot)
{
  const auto root = make_export_test_root("uses_game_package");

  write_text_file(
      root / "game.package.json",
      R"({
        "name": "package-game",
        "version": "0.1.0",
        "entry_scene": "main",
        "asset_root": "game-assets",
        "output_dir": "release"
      })");

  write_text_file(
      root / "game-assets" / "player.txt",
      "player asset");

  vix::game::GameExportConfig config;
  config.project_root = root;
  config.output_directory = root / "dist";
  config.name = "ignored-config-name";
  config.asset_directory = "ignored-assets";

  vix::game::GameExporter exporter;

  auto result = exporter.export_project(config);

  ASSERT_TRUE(result);

  EXPECT_EQ(result.value().output_path, root / "release" / "package-game");
  EXPECT_TRUE(file_exists(root / "release" / "package-game" / "game.package.json"));
  EXPECT_TRUE(file_exists(root / "release" / "package-game" / "game-assets" / "player.txt"));

  EXPECT_FALSE(directory_exists(root / "dist"));
}

TEST(GameExporterTests, GeneratesExportManifest)
{
  const auto root = make_export_test_root("export_manifest");

  write_text_file(
      root / "game.package.json",
      R"({
        "name": "manifest-game",
        "version": "1.0.0",
        "entry_scene": "main",
        "asset_root": "assets",
        "output_dir": "dist"
      })");

  vix::game::GameExportConfig config;
  config.project_root = root;
  config.output_directory = root / "ignored-dist";
  config.name = "ignored-name";

  vix::game::GameExporter exporter;

  auto result = exporter.export_project(config);

  ASSERT_TRUE(result);

  const auto manifest_path =
      root / "dist" / "manifest-game" / "export.json";

  ASSERT_TRUE(file_exists(manifest_path));

  auto json = read_json_file(manifest_path);

  EXPECT_EQ(json["name"].template get<std::string>(), "manifest-game");
  EXPECT_EQ(json["version"].template get<std::string>(), "1.0.0");
  EXPECT_EQ(json["asset_root"].template get<std::string>(), "assets");
  EXPECT_EQ(json["copied_files"].template get<std::uint64_t>(), result.value().copied_files);
  EXPECT_TRUE(json.contains("build_date"));
  EXPECT_TRUE(json["build_date"].is_string());
  EXPECT_FALSE(json["build_date"].template get<std::string>().empty());
}

TEST(GameAssetPipelineTests, ScansAndCountsAssets)
{
  const auto root = make_export_test_root("asset_pipeline_scan_count");

  write_text_file(root / "assets" / "player.png", "fake png");
  write_text_file(root / "assets" / "data" / "level.json", "{}");
  write_text_file(root / "assets" / "audio" / "theme.bin", "binary");

  vix::game::GameAssetPipeline pipeline;

  auto assets = pipeline.scan_assets(root, "assets");

  ASSERT_TRUE(assets);
  EXPECT_EQ(assets.value().size(), 3U);
}

TEST(GameAssetPipelineTests, DetectsImageTextAndBinaryAssets)
{
  EXPECT_EQ(
      vix::game::GameAssetPipeline::detect_type("player.png"),
      vix::game::GameExportAssetType::Image);

  EXPECT_EQ(
      vix::game::GameAssetPipeline::detect_type("level.json"),
      vix::game::GameExportAssetType::Text);

  EXPECT_EQ(
      vix::game::GameAssetPipeline::detect_type("music.dat"),
      vix::game::GameExportAssetType::Binary);
}

TEST(GameAssetPipelineTests, IgnoresTemporaryAndCacheFiles)
{
  const auto root = make_export_test_root("asset_pipeline_ignore_temp_cache");

  write_text_file(root / "assets" / "player.png", "fake png");
  write_text_file(root / "assets" / "debug.log", "log");
  write_text_file(root / "assets" / "temp.tmp", "tmp");
  write_text_file(root / "assets" / "cache" / "cached.txt", "cache");
  write_text_file(root / "assets" / "build" / "generated.txt", "build");

  vix::game::GameAssetPipeline pipeline;

  auto assets = pipeline.scan_assets(root, "assets");

  ASSERT_TRUE(assets);
  ASSERT_EQ(assets.value().size(), 1U);
  EXPECT_EQ(assets.value().front().path, "player.png");
  EXPECT_EQ(assets.value().front().type, vix::game::GameExportAssetType::Image);
}

TEST(GameExporterTests, WritesExportedAssetsIntoManifest)
{
  const auto root = make_export_test_root("export_manifest_assets");

  write_text_file(
      root / "game.package.json",
      R"({
        "name": "asset-manifest-game",
        "version": "1.0.0",
        "entry_scene": "main",
        "asset_root": "assets",
        "output_dir": "dist"
      })");

  write_text_file(root / "assets" / "player.png", "fake png");
  write_text_file(root / "assets" / "levels" / "level.json", "{}");
  write_text_file(root / "assets" / "data.bin", "binary");

  vix::game::GameExportConfig config;
  config.project_root = root;
  config.output_directory = root / "ignored-dist";
  config.name = "ignored-name";

  vix::game::GameExporter exporter;

  auto result = exporter.export_project(config);

  ASSERT_TRUE(result);

  const auto manifest_path =
      root / "dist" / "asset-manifest-game" / "export.json";

  ASSERT_TRUE(file_exists(manifest_path));

  auto json = read_json_file(manifest_path);

  ASSERT_TRUE(json.contains("assets"));
  ASSERT_TRUE(json["assets"].is_array());
  ASSERT_EQ(json["assets"].size(), 3U);

  bool found_image = false;
  bool found_text = false;
  bool found_binary = false;

  for (const auto &asset : json["assets"])
  {
    const auto path = asset["path"].template get<std::string>();
    const auto type = asset["type"].template get<std::string>();

    if (path == "player.png" && type == "image")
    {
      found_image = true;
    }

    if (path == "levels/level.json" && type == "text")
    {
      found_text = true;
    }

    if (path == "data.bin" && type == "binary")
    {
      found_binary = true;
    }
  }

  EXPECT_TRUE(found_image);
  EXPECT_TRUE(found_text);
  EXPECT_TRUE(found_binary);
}
