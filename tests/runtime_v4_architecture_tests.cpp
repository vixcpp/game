/**
 *
 *  @file runtime_v4_architecture_tests.cpp
 *  @author Gaspard Kirira
 *
 *  Vix.cpp
 *
 */

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include <vix/game/App.hpp>
#include <vix/game/AppConfig.hpp>
#include <vix/game/Asset.hpp>
#include <vix/game/AssetManager.hpp>
#include <vix/game/AssetPath.hpp>
#include <vix/game/AssetType.hpp>
#include <vix/game/Camera2D.hpp>
#include <vix/game/DrawSpriteCommand.hpp>
#include <vix/game/EditorContext.hpp>
#include <vix/game/EntityId.hpp>
#include <vix/game/GameRuntime.hpp>
#include <vix/game/InputButton.hpp>
#include <vix/game/InputKey.hpp>
#include <vix/game/InputSystem.hpp>
#include <vix/game/NullRenderer.hpp>
#include <vix/game/NullWindow.hpp>
#include <vix/game/Registry.hpp>
#include <vix/game/Renderer.hpp>
#include <vix/game/Renderer2D.hpp>
#include <vix/game/Scene.hpp>
#include <vix/game/SceneRuntime.hpp>
#include <vix/game/Sprite.hpp>
#include <vix/game/TextureHandle.hpp>
#include <vix/game/Transform2D.hpp>
#include <vix/game/Window.hpp>
#include <vix/game/WindowConfig.hpp>
#include <vix/game/WindowEvent.hpp>
#include <vix/game/RuntimeDiagnostics.hpp>
#include <vix/time/Duration.hpp>
#include <vix/game/EditorRuntime.hpp>
#include <vix/json/json.hpp>

namespace
{
  class CountingScene final : public vix::game::Scene
  {
  public:
    int updates{0};
    int fixed_updates{0};

    void on_update(const vix::game::Frame &frame) override
    {
      (void)frame;
      ++updates;
    }

    void on_fixed_update(const vix::game::Frame &frame) override
    {
      (void)frame;
      ++fixed_updates;
    }
  };

  class HelperScene final : public vix::game::Scene
  {
  public:
    bool saw_input{false};
    bool saw_renderer2d{false};

    void on_update(const vix::game::Frame &frame) override
    {
      (void)frame;

      saw_input = &input() != nullptr;
      saw_renderer2d = &renderer2d() != nullptr;
    }
  };

  class EmptySystem final : public vix::game::System
  {
  public:
    EmptySystem()
        : vix::game::System("empty")
    {
    }
  };

  std::filesystem::path make_test_asset_root()
  {
    const auto root =
        std::filesystem::temp_directory_path() / "vix_game_runtime_v4_assets";

    std::filesystem::create_directories(root);
    return root;
  }

  void write_binary_file(const std::filesystem::path &path)
  {
    std::ofstream file(path, std::ios::binary);
    file << "fake image bytes";
  }

  class RegistryScene final : public vix::game::Scene
  {
  public:
    vix::game::GameBoolResult on_load() override
    {
      auto entity = registry_.create_entity("player");

      auto component = registry_.emplace_component<vix::game::Transform2D>(
          entity.id(),
          vix::game::Transform2D::identity());

      if (!component)
      {
        return component.error();
      }

      return vix::game::Scene::on_load();
    }

    vix::game::Registry *registry() noexcept override
    {
      return &registry_;
    }

    const vix::game::Registry *registry() const noexcept override
    {
      return &registry_;
    }

  private:
    vix::game::Registry registry_{};
  };
}

TEST(GameRuntimeV4ArchitectureTests, RuntimeUpdatesActiveScene)
{
  vix::game::App app;

  vix::game::GameRuntime runtime(app);

  auto init_result = runtime.init();
  ASSERT_TRUE(init_result);

  auto scene_id = app.scenes().create<CountingScene>("main");
  ASSERT_TRUE(scene_id);

  auto active = app.scenes().set_active("main");
  ASSERT_TRUE(active);

  auto *scene = dynamic_cast<CountingScene *>(app.scenes().active());
  ASSERT_NE(scene, nullptr);

  vix::game::Frame frame;
  frame.index = 1;

  runtime.update(frame);
  runtime.fixed_update(frame);

  EXPECT_EQ(scene->updates, 1);
  EXPECT_EQ(scene->fixed_updates, 1);
}

TEST(GameRuntimeV4ArchitectureTests, InputSystemHandlesWindowEvents)
{
  vix::game::InputSystem input;

  input.begin_frame();

  input.handle_window_event(
      vix::game::WindowEvent::key_pressed(vix::game::InputKey::Space));

  EXPECT_TRUE(input.key_down(vix::game::InputKey::Space));
  EXPECT_TRUE(input.key_pressed(vix::game::InputKey::Space));

  input.begin_frame();

  input.handle_window_event(
      vix::game::WindowEvent::key_released(vix::game::InputKey::Space));

  EXPECT_FALSE(input.key_down(vix::game::InputKey::Space));
  EXPECT_TRUE(input.key_released(vix::game::InputKey::Space));

  input.handle_window_event(
      vix::game::WindowEvent::button_pressed(vix::game::InputButton::Left));

  EXPECT_TRUE(input.button_down(vix::game::InputButton::Left));
  EXPECT_TRUE(input.button_pressed(vix::game::InputButton::Left));

  input.handle_window_event(
      vix::game::WindowEvent::pointer_moved(10.0, 20.0, 3.0, 4.0));

  auto pointer = input.pointer_position();
  auto delta = input.pointer_delta();

  EXPECT_DOUBLE_EQ(pointer.x, 10.0);
  EXPECT_DOUBLE_EQ(pointer.y, 20.0);
  EXPECT_DOUBLE_EQ(delta.x, 3.0);
  EXPECT_DOUBLE_EQ(delta.y, 4.0);

  input.handle_window_event(
      vix::game::WindowEvent::wheel_moved(0.0, 1.0));

  auto wheel = input.wheel_delta();

  EXPECT_DOUBLE_EQ(wheel.x, 0.0);
  EXPECT_DOUBLE_EQ(wheel.y, 1.0);
}

TEST(GameRuntimeV4ArchitectureTests, Renderer2DFlushesSpritesToNullRenderer)
{
  vix::game::Window window(std::make_unique<vix::game::NullWindow>());

  auto opened = window.open(vix::game::WindowConfig::defaults());
  ASSERT_TRUE(opened);

  auto null_backend = std::make_unique<vix::game::NullRenderer>();
  auto *raw_backend = null_backend.get();

  vix::game::Renderer renderer(std::move(null_backend));

  auto initialized = renderer.init(window);
  ASSERT_TRUE(initialized);

  vix::game::Renderer2D renderer2d(renderer);

  vix::game::Sprite sprite =
      vix::game::Sprite::from_asset(1).set_size(vix::game::Vec2{32.0F, 32.0F});

  vix::game::Transform2D transform =
      vix::game::Transform2D::at(vix::game::Vec2{10.0F, 20.0F});

  renderer2d.begin_frame();
  renderer2d.draw_sprite(transform, sprite);

  EXPECT_EQ(renderer2d.command_count(), 1U);

  renderer2d.end_frame();

  EXPECT_FALSE(renderer2d.frame_active());
  EXPECT_EQ(raw_backend->sprite_count(), 1U);
}

TEST(GameRuntimeV4ArchitectureTests, NullRendererAcceptsTextureUpload)
{
  vix::game::Window window(std::make_unique<vix::game::NullWindow>());

  auto opened = window.open(vix::game::WindowConfig::defaults());
  ASSERT_TRUE(opened);

  vix::game::NullRenderer renderer;

  auto initialized = renderer.init(window);
  ASSERT_TRUE(initialized);

  vix::game::Asset asset = vix::game::Asset::binary(
      1,
      vix::game::AssetType::Image,
      vix::game::AssetPath("assets", "player.png"),
      vix::game::AssetBytes{1, 2, 3, 4});

  auto uploaded = renderer.upload_texture(asset);
  ASSERT_TRUE(uploaded);

  EXPECT_EQ(renderer.uploaded_texture_count(), 1U);
}

TEST(GameRuntimeV4ArchitectureTests, AssetManagerLoadOrGetReturnsExistingId)
{
  const auto root = make_test_asset_root();
  const auto asset_file = root / "player.bin";

  write_binary_file(asset_file);

  vix::game::AssetManager assets(root.string());

  auto first = assets.load_or_get("player.bin");
  ASSERT_TRUE(first);

  auto second = assets.load_or_get("player.bin");
  ASSERT_TRUE(second);

  EXPECT_EQ(first.value(), second.value());
  EXPECT_EQ(assets.size(), 1U);
}

TEST(GameRuntimeV4ArchitectureTests, SceneHelpersExposeRuntimeContext)
{
  vix::game::App app;

  vix::game::GameRuntime runtime(app);

  auto init_result = runtime.init();
  ASSERT_TRUE(init_result);

  auto scene_id = app.scenes().create<HelperScene>("main");
  ASSERT_TRUE(scene_id);

  auto active = app.scenes().set_active("main");
  ASSERT_TRUE(active);

  auto *scene = dynamic_cast<HelperScene *>(app.scenes().active());
  ASSERT_NE(scene, nullptr);

  vix::game::Frame frame;
  frame.index = 1;

  runtime.update(frame);

  EXPECT_TRUE(scene->saw_input);
  EXPECT_TRUE(scene->saw_renderer2d);
}

TEST(GameRuntimeV4ArchitectureTests, EditorContextTracksSelectedEntity)
{
  vix::game::App app;
  vix::game::GameRuntime runtime(app);

  auto init_result = runtime.init();
  ASSERT_TRUE(init_result);

  vix::game::EditorContext editor(runtime.context());

  constexpr vix::game::EntityId entity_id = 42;

  editor.select_entity(entity_id);

  EXPECT_TRUE(editor.has_selected_entity());
  EXPECT_EQ(editor.selected_entity(), entity_id);

  editor.clear_selected_entity();

  EXPECT_FALSE(editor.has_selected_entity());
  EXPECT_EQ(editor.selected_entity(), vix::game::invalid_entity_id);
}

TEST(GameRuntimeV4ArchitectureTests, RegistryStatsReportsRuntimeCounts)
{
  vix::game::Registry registry;

  auto entity = registry.create_entity("player");
  ASSERT_TRUE(entity.valid());

  auto component = registry.emplace_component<vix::game::Transform2D>(
      entity.id(),
      vix::game::Transform2D::identity());

  ASSERT_TRUE(component);

  auto system_result = registry.create_system<EmptySystem>();
  ASSERT_TRUE(system_result);

  auto stats = registry.stats();

  EXPECT_EQ(stats.entity_count, 1U);
  EXPECT_EQ(stats.system_count, 1U);
  EXPECT_EQ(stats.component_store_count, 1U);
}

TEST(GameRuntimeV4ArchitectureTests, SceneRuntimeInspectReturnsSceneSnapshot)
{
  vix::game::App app;
  vix::game::GameRuntime runtime(app);

  auto init_result = runtime.init();
  ASSERT_TRUE(init_result);

  vix::game::SceneRuntime scenes(runtime.context());

  auto scene_id = scenes.create<CountingScene>("main");
  ASSERT_TRUE(scene_id);

  auto active = scenes.activate("main");
  ASSERT_TRUE(active);

  auto data = scenes.inspect();

  EXPECT_EQ(data.active_scene, "main");
  ASSERT_EQ(data.scenes.size(), 1U);
  EXPECT_EQ(data.scenes.front().name, "main");
  EXPECT_TRUE(data.scenes.front().loaded);
  EXPECT_TRUE(data.scenes.front().active);
}

TEST(GameRuntimeV4ArchitectureTests, AppConfigParsesWindowConfig)
{
  vix::json::Json json = vix::json::Json::object();

  json["name"] = "v4_game";
  json["title"] = "V4 Game";
  json["asset_root"] = "assets";
  json["start_scene"] = "main";
  json["headless"] = false;

  vix::json::Json window = vix::json::Json::object();
  window["title"] = "V4 Window";
  window["width"] = 1024;
  window["height"] = 768;
  window["fullscreen"] = false;
  window["resizable"] = true;
  window["visible"] = true;
  window["vsync"] = true;

  json["window"] = window;

  auto config = vix::game::AppConfig::from_json(json);
  ASSERT_TRUE(config);

  EXPECT_FALSE(config.value().headless);
  EXPECT_EQ(config.value().window.title, "V4 Window");
  EXPECT_EQ(config.value().window.width, 1024U);
  EXPECT_EQ(config.value().window.height, 768U);
  EXPECT_TRUE(config.value().window.resizable);
  EXPECT_TRUE(config.value().window.visible);
  EXPECT_TRUE(config.value().window.vsync);

  auto serialized = config.value().to_json();

  ASSERT_TRUE(serialized.contains("window"));
  EXPECT_EQ(serialized["window"]["title"].template get<std::string>(), "V4 Window");
}

TEST(GameRuntimeV4ArchitectureTests, GameContextInstallsWindowAndRendererBackends)
{
  vix::game::App app;
  vix::game::GameRuntime runtime(app);

  auto init_result = runtime.init();
  ASSERT_TRUE(init_result);

  auto &context = runtime.context();

  context
      .set_window_backend(std::make_unique<vix::game::NullWindow>())
      .set_renderer_backend(std::make_unique<vix::game::NullRenderer>());

  EXPECT_TRUE(context.window().has_backend());
  EXPECT_TRUE(context.renderer().has_backend());
  EXPECT_EQ(context.window().backend_name(), std::string("null"));
  EXPECT_EQ(context.renderer().backend_name(), std::string("null"));

  auto opened = context.window().open(vix::game::WindowConfig::headless_config());
  ASSERT_TRUE(opened);

  auto renderer_init = context.renderer().init(context.window());
  ASSERT_TRUE(renderer_init);

  EXPECT_TRUE(context.window().open());
  EXPECT_TRUE(context.renderer().initialized());
  EXPECT_TRUE(context.renderer2d().attached());
}

TEST(GameRuntimeV4ArchitectureTests, RuntimeDiagnosticsReportsCoreState)
{
  vix::game::App app;
  vix::game::GameRuntime runtime(app);

  auto init_result = runtime.init();
  ASSERT_TRUE(init_result);

  auto &context = runtime.context();

  context
      .set_window_backend(std::make_unique<vix::game::NullWindow>())
      .set_renderer_backend(std::make_unique<vix::game::NullRenderer>());

  auto opened = context.window().open(vix::game::WindowConfig::headless_config());
  ASSERT_TRUE(opened);

  auto renderer_init = context.renderer().init(context.window());
  ASSERT_TRUE(renderer_init);

  auto scene = app.scenes().create<CountingScene>("main");
  ASSERT_TRUE(scene);

  auto active = app.scenes().set_active("main");
  ASSERT_TRUE(active);

  vix::game::Frame frame;
  frame.index = 42;
  frame.delta = vix::time::Duration::milliseconds(16);

  runtime.begin_frame(frame);
  runtime.update(frame);
  runtime.render(frame);
  runtime.end_frame(frame);

  auto diagnostics = runtime.diagnostics();

  EXPECT_EQ(diagnostics.frame_index, 42U);
  EXPECT_EQ(diagnostics.delta_ms, 16);
  EXPECT_EQ(diagnostics.window_backend, "null");
  EXPECT_EQ(diagnostics.renderer_backend, "null");
  EXPECT_EQ(diagnostics.window_width, 1U);
  EXPECT_EQ(diagnostics.window_height, 1U);
  EXPECT_EQ(diagnostics.scene_count, 1U);
  EXPECT_EQ(diagnostics.active_scene, "main");
  EXPECT_TRUE(diagnostics.has_window_backend());
  EXPECT_TRUE(diagnostics.has_renderer_backend());
  EXPECT_TRUE(diagnostics.has_active_scene());
}

TEST(GameRuntimeV4ArchitectureTests, EditorRuntimeInspectsRuntimeScenesAndRegistry)
{
  vix::game::App app;
  vix::game::GameRuntime runtime(app);

  auto init_result = runtime.init();
  ASSERT_TRUE(init_result);

  auto &context = runtime.context();

  context
      .set_window_backend(std::make_unique<vix::game::NullWindow>())
      .set_renderer_backend(std::make_unique<vix::game::NullRenderer>());

  auto opened = context.window().open(vix::game::WindowConfig::headless_config());
  ASSERT_TRUE(opened);

  auto renderer_init = context.renderer().init(context.window());
  ASSERT_TRUE(renderer_init);

  auto scene = app.scenes().create<RegistryScene>("main");
  ASSERT_TRUE(scene);

  auto active = app.scenes().set_active("main");
  ASSERT_TRUE(active);

  vix::game::EditorRuntime editor(runtime);

  auto editor_init = editor.init();
  ASSERT_TRUE(editor_init);

  vix::game::Frame frame;
  frame.index = 7;
  frame.delta = vix::time::Duration::milliseconds(16);

  editor.begin_frame(frame);
  editor.render(frame);
  editor.end_frame(frame);

  auto runtime_info = editor.inspect_runtime();
  EXPECT_EQ(runtime_info.frame_index, 7U);
  EXPECT_EQ(runtime_info.window_backend, "null");
  EXPECT_EQ(runtime_info.renderer_backend, "null");

  auto scenes = editor.inspect_scenes();
  EXPECT_EQ(scenes.active_scene, "main");
  ASSERT_EQ(scenes.scenes.size(), 1U);
  EXPECT_EQ(scenes.scenes.front().name, "main");

  auto registry = editor.inspect_registry();
  EXPECT_EQ(registry.entity_count, 1U);
  EXPECT_EQ(registry.component_store_count, 1U);
}
