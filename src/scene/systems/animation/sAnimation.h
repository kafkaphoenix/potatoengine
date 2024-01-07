#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/application.h"
#include "core/time.h"
#include "scene/components/common/cName.h"
#include "scene/components/common/cTag.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/graphics/cTextureAtlas.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"

namespace potatoengine {

void animationSystem(entt::registry& registry, const Time& ts) {
  const auto& settings = Application::Get().getSettings();

  registry.view<CTransform, CRigidBody, CUUID>().each(
    [&](entt::entity e, CTransform& cTransform, const CRigidBody& cRigidBody,
        const CUUID& cUUID) {
      if (cRigidBody.isKinematic) {
        if (settings->activeScene == "Cubes") {
          const auto& cTag = registry.get<CTag>(e);
          if (cTag.tag.ends_with("_block")) {
            cTransform.rotate(1.f, {0.f, 1.f, 0.f});
          }
        } else if (settings->activeScene == "Flappy Bird") {
          const CName& cName = registry.get<CName>(e);

          if (cName.name == "bird") {
            CTextureAtlas& cTextureAtlas = registry.get<CTextureAtlas>(e);

            static int birdState = 2;
            static double delay = 0;
            delay += ts;

            if (delay > 0.1) {
              delay = 0;
              cTextureAtlas.index = birdState;
              birdState--;
              if (birdState < 0) {
                birdState = 2;
              }
            }

            static float rotation = 0.0f;
            static bool rotate = true;
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
