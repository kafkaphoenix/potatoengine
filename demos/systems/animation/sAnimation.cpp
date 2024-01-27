#include "systems/animation/sAnimation.h"

// TODO REMOVE
static int birdState = 2;
static int coinState = 0;
static double birdDelay = 0;
static double coinDelay = 0;
static float rotation = 0.0f;
static bool rotate = true;

namespace demos::systems {

void AnimationSystem::update(entt::registry& registry, const engine::Time& ts) {
  auto& app = engine::Application::Get();
  if (app.isGamePaused()) {
    return;
  }
  const auto& settings = app.getSettings();

  registry.view<engine::CTransform, engine::CRigidBody, engine::CTag, engine::CUUID>().each(
    [&](entt::entity e, engine::CTransform& cTransform,
        const engine::CRigidBody& cRigidBody, const engine::CTag& cTag, const engine::CUUID& cUUID) {
      if (cRigidBody.isKinematic) {
        if (settings->activeScene == "Cubes") {
          if (cTag.tag.ends_with("_block")) {
            cTransform.rotate(1.f, {0.f, 1.f, 0.f});
          }
        } else if (settings->activeScene == "Flappy Bird") {
          if (cTag.tag == "bird") {
            engine::CTextureAtlas& cTextureAtlas =
              registry.get<engine::CTextureAtlas>(e);

            birdDelay += ts;

            if (birdDelay > 0.1) {
              birdDelay = 0;
              cTextureAtlas.index = birdState;
              birdState--;
              if (birdState < 0) {
                birdState = 2;
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
              cTextureAtlas.index = coinState;
              coinState++;
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
