#pragma once

#include <glm/glm.hpp>

#include "potatoengine/core/time.h"
#include "potatoengine/renderer/renderer.h"
#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/physics/cRigidBody.h"
#include "potatoengine/scene/components/world/cSkybox.h"
#include "potatoengine/scene/components/physics/cTransform.h"
#include "potatoengine/scene/components/world/cTime.h"
#include <entt/entt.hpp>

namespace potatoengine {

void movementSystem(entt::registry& r, Time ts) {
    r.view<CTransform, CRigidBody, CUUID>().each([&](entt::entity e, CTransform& cTransform, const CRigidBody& cRigidBody, const CUUID& cUUID) {
        CSkybox* cSkybox = r.try_get<CSkybox>(e);

        if (cRigidBody.isKinematic) {
            // TODO sun movement system
            // TODO real movement system
            cTransform.rotation = glm::angleAxis(glm::radians(1.f), glm::vec3(0.f, 1.f, 0.f)) * cTransform.rotation;
            // glm quatLookAt allows you to construct quaternion from a unit vector of a direction you want to rotate
        }

        if (cSkybox) {
            const CTime& cTime = r.get<CTime>(e);
            float rotation = 0.f;
            if (cSkybox->rotationSpeed > 0.f) {
                rotation = cSkybox->rotationSpeed;
            } else {
                // to avoid rotating fps frames per second instead of one we need to divide by fps
                float rotationAnglePerSecond = (360.f / (cTime.dayLength * 3600.f)) / cTime.fps;
                rotation = rotationAnglePerSecond * cTime.acceleration;
            }
            cTransform.rotation = glm::angleAxis(glm::radians(rotation), glm::vec3(0.f, 1.f, 0.f)) * cTransform.rotation;
        }
    });
}
}
