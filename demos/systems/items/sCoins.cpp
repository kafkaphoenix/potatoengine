#include "systems/items/sCoins.h"

#include "components/config/CCoins.h"

namespace demos::systems {

CoinsSystem::~CoinsSystem() {
  auto& registry = engine::Application::Get().getSceneManager()->getRegistry();
  for (auto e : registry.view<engine::CTag, engine::CUUID>()) {
    if (registry.get<engine::CTag>(e).tag == "coin") {
      registry.destroy(e);
    }
  }
}

void CoinsSystem::init(entt::registry& registry) {
  const auto& scene_manager = engine::Application::Get().getSceneManager();

  entt::entity gamestate = registry.view<CCoins, engine::CUUID>().front();
  CCoins& coins_config = registry.get<CCoins>(gamestate);

  for (uint32_t i = 0; i < coins_config.maxCoins; i++) {
    entt::entity coin_ =
      scene_manager->createEntity("scene", "coin", "coin_" + std::to_string(i));
    registry.get<engine::CShaderProgram>(coin_).isVisible = false;
  }
  coins_config.coins = coins_config.maxCoins;
}

void CoinsSystem::update(entt::registry& registry, const engine::Time& ts) {
  if (engine::Application::Get().isGamePaused()) {
    return;
  }

  entt::entity gamestate = registry.view<CCoins, engine::CUUID>().front();
  CCoins& coins_config = registry.get<CCoins>(gamestate);

  if (coins_config.coins == 0) {
    coins_config.coins = coins_config.maxCoins;
  }

  registry
    .view<engine::CShaderProgram, engine::CTransform, engine::CTag,
          engine::CUUID>()
    .each([&](engine::CShaderProgram& cShaderProgram,
              engine::CTransform& cTransform, const engine::CTag& cTag,
              const engine::CUUID& cUUID) {
      if (cTag.tag == "coin") {
        if (cShaderProgram.isVisible) {
          float speed = 0.005f; // TODO move to component

          // move coin
          cTransform.position.x -= speed;

          // check if coin is out of screen
          if (cTransform.position.x < -2.f) {
            cShaderProgram.isVisible = false;
          }
        } else {
          if (coins_config.coins > 0) {
            // TODO check if there is a pipe already in the same position
            // randomize y position
            // -0.5 highest 0.6 lowest
            cTransform.position.y =
              -0.5f + static_cast<float>(rand()) /
                        (static_cast<float>(RAND_MAX / 1.1f));
            cTransform.position.x = 2.f;
            cShaderProgram.isVisible = true;
            coins_config.coins--;
          }
        }
      }
    });
}
}