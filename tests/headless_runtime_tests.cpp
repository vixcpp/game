/**
 *
 *  @file headless_runtime_tests.cpp
 *  @author Gaspard Kirira
 *
 *  Vix.cpp
 *
 */

#include <gtest/gtest.h>

#include <memory>

#include <vix/game/App.hpp>
#include <vix/game/AppConfig.hpp>
#include <vix/game/Frame.hpp>
#include <vix/game/GameRuntime.hpp>
#include <vix/game/NullRenderer.hpp>
#include <vix/game/NullWindow.hpp>
#include <vix/game/Renderer.hpp>
#include <vix/game/Renderer2D.hpp>
#include <vix/game/Scene.hpp>
#include <vix/game/Sprite.hpp>
#include <vix/game/TextureHandle.hpp>
#include <vix/game/Transform2D.hpp>
#include <vix/game/Vec2.hpp>
#include <vix/game/Window.hpp>
#include <vix/game/WindowConfig.hpp>

namespace
{
  class HeadlessScene final : public vix::game::Scene
  {
  public:
    int updates{0};
    int fixed_updates{0};

    void on_update(const vix::game::Frame &frame) override
    {
      (void)frame;

      ++updates;

      auto sprite = vix::game::Sprite::from_asset(1);
      sprite.set_size(vix::game::Vec2{32.0F, 32.0F});

      auto transform = vix::game::Transform2D::at(
          vix::game::Vec2{10.0F, 20.0F});

      renderer2d().draw_sprite(transform, sprite);
    }

    void on_fixed_update(const vix::game::Frame &frame) override
    {
      (void)frame;
      ++fixed_updates;
    }
  };
}

TEST(HeadlessRuntimeTests, AppRuntimeWorksWithNullWindowAndNullRenderer)
{
  vix::game::AppConfig config = vix::game::AppConfig::defaults();
  config.headless = true;
  config.window = vix::game::WindowConfig::headless_config();

  vix::game::App app(config);

  auto init = app.init();
  ASSERT_TRUE(init);

  auto &runtime = app.runtime();
  auto &context = runtime.context();

  context.window().set_backend(std::make_unique<vix::game::NullWindow>());

  auto opened = context.window().open(vix::game::WindowConfig::headless_config());
  ASSERT_TRUE(opened);

  auto null_renderer = std::make_unique<vix::game::NullRenderer>();
  auto *raw_renderer = null_renderer.get();

  context.renderer().set_backend(std::move(null_renderer));

  auto renderer_init = context.renderer().init(context.window());
  ASSERT_TRUE(renderer_init);

  auto scene_id = app.scenes().create<HeadlessScene>("main");
  ASSERT_TRUE(scene_id);

  auto active = app.scenes().set_active("main");
  ASSERT_TRUE(active);

  vix::game::Frame frame;
  frame.index = 1;

  runtime.begin_frame(frame);
  runtime.update(frame);
  runtime.fixed_update(frame);
  runtime.render(frame);
  runtime.end_frame(frame);

  auto *scene = dynamic_cast<HeadlessScene *>(app.scenes().active());
  ASSERT_NE(scene, nullptr);

  EXPECT_EQ(scene->updates, 1);
  EXPECT_EQ(scene->fixed_updates, 1);

  EXPECT_TRUE(context.window().open());
  EXPECT_TRUE(context.renderer().initialized());
  EXPECT_EQ(context.window().backend_name(), std::string("null"));
  EXPECT_EQ(context.renderer().backend_name(), std::string("null"));

  EXPECT_FALSE(context.renderer2d().frame_active());
  EXPECT_EQ(raw_renderer->sprite_count(), 1U);
}
