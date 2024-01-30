#include "systems/animation/sAnimation.h"

// TODO REMOVE
static uint32_t birdState{};
static uint32_t coinState{};
static float birdDelay{};
static float coinDelay{};
static float rotation{};
static bool rotate = true;

namespace demos::systems {

void AnimationSystem::update(entt::registry& registry, const engine::Time& ts) {
  auto& app = engine::Application::Get();
  if (app.isGamePaused()) {
    return;
  }
  const auto& settings_manager = app.getSettingsManager();

  registry
    .view<engine::CTransform, engine::CRigidBody, engine::CTag, engine::CUUID>()
    .each([&](entt::entity e, engine::CTransform& cTransform,
              const engine::CRigidBody& cRigidBody, const engine::CTag& cTag,
              const engine::CUUID& cUUID) {
      if (cRigidBody.isKinematic) {
        if (settings_manager->activeScene == "Cubes") {
          if (cTag.tag.ends_with("_block")) {
            cTransform.rotate(1.f, {0.f, 1.f, 0.f});
          }
        } else if (settings_manager->activeScene == "Flappy Bird") {
          if (cTag.tag == "bird") {
            engine::CTextureAtlas& cTextureAtlas =
              registry.get<engine::CTextureAtlas>(e);

            birdDelay += ts;

            if (birdDelay > 0.1) {
              birdDelay = 0;
              cTextureAtlas.index = birdState++;
              if (birdState > 2) {
                birdState = 0;
              }
            }

            if (rotate) {
              rotation += 0.1f;
              if (rotation > 0.5f) {
                rotate = false;
              }
            } else {
              rotation -= 0.1f;
              if (rotation < -0.5f) {
                rotate = true;
              }
            }
            cTransform.rotate(rotation, {0.0f, 0.0f, 1.0f});
          } else if (cTag.tag == "coin") {
            engine::CTextureAtlas& cTextureAtlas =
              registry.get<engine::CTextureAtlas>(e);

            coinDelay += ts / 4; // TODO should depends on a value speed

            if (coinDelay > 0.1) {
              coinDelay = 0;
              cTextureAtlas.index = coinState++;
              if (coinState > 5) {
                coinState = 0;
              }
            }
          }
        }
      }
    });
}
}
