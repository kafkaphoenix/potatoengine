#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/application.h"
#include "core/input.h"
#include "core/keyCodes.h"
#include "core/time.h"
#include "scene/components/camera/cActiveCamera.h"
#include "scene/components/camera/cCamera.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/input/cActiveInput.h"
#include "scene/components/input/cInput.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"

namespace potatoengine {

void movementSystem(entt::registry& registry, const Time& ts) {
  if (Application::Get().isDebugging()) {
    return;
  }

  registry.view<CTransform, CRigidBody, CActiveInput, CInput, CUUID>().each(
    [&](entt::entity e, CTransform& cTransform, const CRigidBody& cRigidBody,
        const CActiveInput& cActiveInput, const CInput& cInput,
        const CUUID& cUUID) {
      if (cRigidBody.isKinematic) {
        if (cInput.mode == CInput::Mode::_3D) {
          CCamera* cCamera = registry.try_get<CCamera>(e);
          ENGINE_ASSERT(cCamera,
                        "Input is 3D mode but entity is missing a camera!");
          if (cCamera->mode == CCamera::Mode::_3D) {
            CActiveCamera* cActiveCamera = registry.try_get<CActiveCamera>(e);
            ENGINE_ASSERT(
              cActiveCamera,
              "Input and camera are 3D mode but entity camera is not "
              "the active camera!");

            float speed = ts * cInput.translationSpeed;

            glm::quat qF = cTransform.rotation * glm::quat(0, 0, 0, -1) *
                           glm::conjugate(cTransform.rotation);
            glm::vec3 front = glm::normalize(glm::vec3(qF.x, qF.y, qF.z));
            glm::vec3 right =
              glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
            glm::vec3 up = {0, ts * cInput.verticalSpeed, 0};

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
          } else if (cCamera->mode == CCamera::Mode::_2D) {
            // TODO: static camera?
          }
        } else if (cInput.mode == CInput::Mode::_2D) {
          float speed = ts * cInput.translationSpeed;

          if (Input::IsKeyPressed(Key::Space)) {
            cTransform.position += glm::vec3(0, 1, 0) * speed;
          }
        }
      }
    });
}
}
