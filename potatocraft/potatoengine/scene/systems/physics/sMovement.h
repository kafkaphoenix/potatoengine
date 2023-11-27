#pragma once

#include <glm/glm.hpp>

#include "potatoengine/core/time.h"
#include "potatoengine/renderer/renderer.h"
#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/physics/cRigidBody.h"
#include "potatoengine/scene/components/world/cSkybox.h"
#include "potatoengine/scene/components/physics/cTransform.h"
#include "potatoengine/scene/components/world/cTime.h"
#include "potatoengine/scene/components/camera/cCamera.h"
#include "potatoengine/scene/components/camera/cActiveCamera.h"
#include "potatoengine/core/input.h"
#include "potatoengine/core/keyCodes.h"
#include <entt/entt.hpp>

namespace potatoengine {

void movementSystem(entt::registry& r, Time ts) {
    r.view<CTransform, CRigidBody, CUUID>().each([&](entt::entity e, CTransform& cTransform, const CRigidBody& cRigidBody, const CUUID& cUUID) {
        CSkybox* cSkybox = r.try_get<CSkybox>(e);
        CCamera* cCamera = r.try_get<CCamera>(e);

        if (cCamera) {
            CActiveCamera* cActiveCamera = r.try_get<CActiveCamera>(e);
            if (cActiveCamera and cRigidBody.isKinematic) {
                CCamera* cCamera = r.try_get<CCamera>(e);
                float speed = ts * cCamera->translationSpeed;

                glm::quat qF = cTransform.rotation * glm::quat(0, 0, 0, -1) * glm::conjugate(cTransform.rotation);
                glm::vec3 front = glm::normalize(glm::vec3(qF.x, qF.y, qF.z));
                glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
                glm::vec3 up = {0, ts * cCamera->verticalSpeed, 0};

                if (Input::IsKeyPressed(Key::W))
                    cTransform.position += front * speed;

                if (Input::IsKeyPressed(Key::S))
                    cTransform.position -= front * speed;

                if (Input::IsKeyPressed(Key::A))
                    cTransform.position -= right * speed;

                if (Input::IsKeyPressed(Key::D))
                    cTransform.position += right * speed;

                if (Input::IsKeyPressed(Key::Space))
                    cTransform.position += up;

                if (Input::IsKeyPressed(Key::LeftControl))
                    cTransform.position -= up;

                cCamera->calculateView(cTransform.position, cTransform.rotation);
            }
        } else if (cSkybox and cRigidBody.isKinematic) {
            // TODO cActiveSkybox and move is kinematic check here 
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
        } else {
            // TODO sun moon movement system
            // TODO this should throw an error think how to make thinks move maybe animation
            // TODO consider this function glm quatLookAt allows you to construct quaternion from a unit vector of a direction you want to rotate
            if (cRigidBody.isKinematic) { // TODO just temporal for cubes testing
                cTransform.rotation = glm::angleAxis(glm::radians(1.f), glm::vec3(0.f, 1.f, 0.f)) * cTransform.rotation;
            }
        }
    });
}
}
