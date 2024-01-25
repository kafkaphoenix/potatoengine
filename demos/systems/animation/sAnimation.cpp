#include "systems/animation/sAnimation.h"

static int birdState = 2;
static double delay = 0;
static float rotation = 0.0f;
static bool rotate = true;

namespace demos::systems {

void AnimationSystem::update(entt::registry& registry, const engine::Time& ts) {
  auto& app = engine::Application::Get();
  if (app.isGamePaused()) {
    return;
  }
  const auto& settings = app.getSettings();

  registry.view<engine::CTransform, engine::CRigidBody, engine::CUUID>().each(
    [&](entt::entity e, engine::CTransform& cTransform,
        const engine::CRigidBody& cRigidBody, const engine::CUUID& cUUID) {
      if (cRigidBody.isKinematic) {
        if (settings->activeScene == "Cubes") {
          const auto& cTag = registry.get<engine::CTag>(e);
          if (cTag.tag.ends_with("_block")) {
            cTransform.rotate(1.f, {0.f, 1.f, 0.f});
          }
        } else if (settings->activeScene == "Flappy Bird") {
          const engine::CName& cName = registry.get<engine::CName>(e);

          if (cName.name == "bird") {
            engine::CTextureAtlas& cTextureAtlas =
              registry.get<engine::CTextureAtlas>(e);

            delay += ts;

            if (delay > 0.1) {
              delay = 0;
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
          }
        }
      }
    });
}
}
