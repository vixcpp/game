// A complete headless-capable reference game built only from vix/game
// primitives. Gameplay-specific types deliberately live in this example.
#include <memory>

#include <vix/game/App.hpp>
#include <vix/game/AppConfig.hpp>
#include <vix/game/Color.hpp>
#include <vix/game/Event.hpp>
#include <vix/game/InputKey.hpp>
#include <vix/game/Registry.hpp>
#include <vix/game/Scene.hpp>
#include <vix/game/Sprite.hpp>
#include <vix/game/System.hpp>
#include <vix/game/Transform2D.hpp>
#include <vix/game/Vec2.hpp>

namespace
{
  struct Player { float speed{180.0F}; };
  struct Coin { bool collected{false}; };

  class MovementSystem final : public vix::game::System
  {
  public:
    explicit MovementSystem(vix::game::InputSystem &input) : input_(input) {}

    void on_update(const vix::game::Frame &frame) override
    {
      const float seconds = static_cast<float>(frame.delta_ms()) / 1000.0F;
      registry().view<Player, vix::game::Transform2D>().each(
          [&](vix::game::Entity, Player &player, vix::game::Transform2D &transform)
          {
            if (input_.action_down("move_left")) transform.position.x -= player.speed * seconds;
            if (input_.action_down("move_right")) transform.position.x += player.speed * seconds;
            if (input_.action_down("move_up")) transform.position.y -= player.speed * seconds;
            if (input_.action_down("move_down")) transform.position.y += player.speed * seconds;
          });
    }
  private:
    vix::game::InputSystem &input_;
  };

  class SpriteRenderSystem final : public vix::game::System
  {
  public:
    explicit SpriteRenderSystem(vix::game::Renderer2D &renderer) : renderer_(renderer) {}

    void on_update(const vix::game::Frame &) override
    {
      renderer_.clear(vix::game::Color{0.05F, 0.06F, 0.09F, 1.0F});
      registry().view<vix::game::Transform2D, vix::game::Sprite>().each(
          [&](vix::game::Entity, vix::game::Transform2D &transform, vix::game::Sprite &sprite)
          {
            renderer_.draw_sprite(transform, sprite);
          });
    }
  private:
    vix::game::Renderer2D &renderer_;
  };

  class TinyAdventureReferenceScene final : public vix::game::Scene
  {
  public:
    [[nodiscard]] vix::game::GameBoolResult on_load() override
    {
      auto player_asset = assets().load_or_get("player.jpg");
      auto coin_asset = assets().load_or_get("coin.jpeg");
      if (!player_asset) return player_asset.error();
      if (!coin_asset) return coin_asset.error();

      auto *player_image = assets().get(player_asset.value());
      auto *coin_image = assets().get(coin_asset.value());
      if (!player_image || !coin_image)
      {
        return vix::game::make_game_error(vix::game::GameErrorCode::AssetNotFound,
                                          "reference game asset disappeared");
      }
      if (auto uploaded = renderer().upload_texture(*player_image); !uploaded) return uploaded.error();
      if (auto uploaded = renderer().upload_texture(*coin_image); !uploaded) return uploaded.error();

      (void)input().bind_key("move_left", vix::game::InputKey::A);
      (void)input().add_key("move_left", vix::game::InputKey::ArrowLeft);
      (void)input().bind_key("move_right", vix::game::InputKey::D);
      (void)input().add_key("move_right", vix::game::InputKey::ArrowRight);
      (void)input().bind_key("move_up", vix::game::InputKey::W);
      (void)input().add_key("move_up", vix::game::InputKey::ArrowUp);
      (void)input().bind_key("move_down", vix::game::InputKey::S);
      (void)input().add_key("move_down", vix::game::InputKey::ArrowDown);
      (void)input().bind_key("quit", vix::game::InputKey::Escape);

      const auto player = registry().create_entity("player");
      (void)registry().emplace_component<Player>(player.id());
      (void)registry().emplace_component<vix::game::Transform2D>(player.id(),
          vix::game::Transform2D::at(vix::game::Vec2{80.0F, 120.0F}));
      auto player_sprite = registry().emplace_component<vix::game::Sprite>(player.id(),
          vix::game::Sprite::from_asset(player_asset.value()));
      if (player_sprite) player_sprite.value()->set_size(vix::game::Vec2{96.0F, 96.0F});

      const auto coin = registry().create_entity("coin");
      (void)registry().emplace_component<Coin>(coin.id());
      (void)registry().emplace_component<vix::game::Transform2D>(coin.id(),
          vix::game::Transform2D::at(vix::game::Vec2{360.0F, 120.0F}));
      auto coin_sprite = registry().emplace_component<vix::game::Sprite>(coin.id(),
          vix::game::Sprite::from_asset(coin_asset.value()));
      if (coin_sprite) coin_sprite.value()->set_size(vix::game::Vec2{64.0F, 64.0F});

      if (auto added = registry().create_system<MovementSystem>(input()); !added) return added.error();
      if (auto added = registry().create_system<SpriteRenderSystem>(renderer2d()); !added) return added.error();

      (void)events().dispatch(vix::game::Event::custom("tiny_adventure_started")
                                  .set_source("tiny_adventure_reference"));
      return vix::game::Scene::on_load();
    }

    void on_update(const vix::game::Frame &frame) override
    {
      if (input().action_pressed("quit") || frame.index >= 2)
      {
        app().stop();
      }
    }
  };
}

int main()
{
  auto config = vix::game::AppConfig::defaults();
  config.headless = true;
  config.asset_root = "examples/assets";
  vix::game::App app(config);
  if (auto scene = app.scenes().create<TinyAdventureReferenceScene>("main"); !scene) return 1;
  if (auto active = app.scenes().set_active("main"); !active) return 1;
  auto result = app.run();
  return result ? 0 : 1;
}
