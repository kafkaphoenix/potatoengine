#pragma once

#include <entt/entt.hpp>

#include "core/application.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/graphics/cTexture.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/world/cSkybox.h"
#include "scene/components/world/cTime.h"

using namespace entt::literals;

namespace potatoengine {

void skyboxSystem(entt::registry& registry) {
  registry.view<CSkybox, CTransform, CRigidBody, CTime, CTexture, CUUID>().each(
    [&](const CSkybox& cSkybox, CTransform& cTransform,
        const CRigidBody& cRigidBody, const CTime& cTime, CTexture& cTexture,
        const CUUID& cUUID) {
      if (cTexture.drawMode == CTexture::DrawMode::TEXTURES_BLEND) {
        float blendFactor = 0.f;
        if (cTime.currentHour >= cTime.nightStart and
            cTime.currentHour < cTime.dayTransitionStart) {
          blendFactor = 0.f;
        } else if (cTime.currentHour >= cTime.dayTransitionStart and
                   cTime.currentHour < cTime.dayStart) {
          blendFactor =
            (cTime.currentMinute +
             (cTime.currentHour - cTime.dayTransitionStart) * cTime.fps) /
            120.f;
        } else if (cTime.currentHour >= cTime.dayStart and
                   cTime.currentHour < cTime.nightTransitionStart) {
          blendFactor = 1.f;
        } else if (cTime.currentHour >= cTime.nightTransitionStart and
                   cTime.currentHour < cTime.nightStart) {
          blendFactor =
            1.f -
            ((cTime.currentMinute +
              (cTime.currentHour - cTime.nightTransitionStart) * cTime.fps) /
             120.f);
        }
        cTexture.blendFactor = blendFactor;
        entt::monostate<"skyBlendFactor"_hs>{} = blendFactor;
        entt::monostate<"useSkyBlending"_hs>{} = 1.f;
      } else {
        entt::monostate<"useSkyBlending"_hs>{} = 0.f;
      }

      if (cSkybox.useFog) {
        entt::monostate<"useFog"_hs>{} = 1.f;
        entt::monostate<"fogDensity"_hs>{} = cSkybox.fogDensity;
        entt::monostate<"fogGradient"_hs>{} = cSkybox.fogGradient;
        entt::monostate<"fogColor"_hs>{} = cSkybox.fogColor;
      } else {
        entt::monostate<"useFog"_hs>{} = 0.f;
      }

      if (cRigidBody.isKinematic) {
        // TODO cActiveSkybox so we can set different ones in different planets
        float rotation = 0.f;
        if (cSkybox.rotationSpeed > 0.f) {
          rotation = cSkybox.rotationSpeed;
        } else {
          // to avoid rotating fps frames per second instead of one we need to
          // divide by fps
          float rotationAnglePerSecond =
            (360.f / (cTime.dayLength * 3600.f)) / cTime.fps;
          rotation = rotationAnglePerSecond * cTime.acceleration;
        }
        cTransform.rotation =
          glm::angleAxis(glm::radians(rotation), glm::vec3(0.f, 1.f, 0.f)) *
          cTransform.rotation;
      }
    });
}
}