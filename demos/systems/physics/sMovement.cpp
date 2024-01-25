#include "systems/physics/sMovement.h"

#include <glm/glm.hpp>

namespace demos::systems {

void MovementSystem::update(entt::registry& registry, const engine::Time& ts) {
  auto& app = engine::Application::Get();
  if (app.isDebugging() or app.isGamePaused()) {
    return;
  }

  registry.view<engine::CTransform, engine::CRigidBody, engine::CActiveInput, engine::CInput, engine::CUUID>().each(
    [&](entt::entity e, engine::CTransform& cTransform, const engine::CRigidBody& cRigidBody,
        const engine::CActiveInput& cActiveInput, const engine::CInput& cInput,
        const engine::CUUID& cUUID) {
      if (cRigidBody.isKinematic) {
        if (cInput.mode == engine::CInput::Mode::_3D) {
          engine::CCamera* cCamera = registry.try_get<engine::CCamera>(e);
          ENGINE_ASSERT(cCamera,
                        "Input is 3D mode but entity is missing a camera!");
          if (cCamera->mode == engine::CCamera::Mode::_3D) {
            engine::CActiveCamera* cActiveCamera = registry.try_get<engine::CActiveCamera>(e);
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

            if (engine::Input::IsKeyPressed(engine::Key::W))
              cTransform.position += front * speed;

            if (engine::Input::IsKeyPressed(engine::Key::S))
              cTransform.position -= front * speed;

            if (engine::Input::IsKeyPressed(engine::Key::A))
              cTransform.position -= right * speed;

            if (engine::Input::IsKeyPressed(engine::Key::D))
              cTransform.position += right * speed;

            if (engine::Input::IsKeyPressed(engine::Key::Space))
              cTransform.position += up;

            if (engine::Input::IsKeyPressed(engine::Key::LeftControl))
              cTransform.position -= up;

            cCamera->calculateView(cTransform.position, cTransform.rotation);
          } else if (cCamera->mode == engine::CCamera::Mode::_2D) {
            // TODO: static camera?
          }
        } else if (cInput.mode == engine::CInput::Mode::_2D) {
          float speed = ts * cInput.translationSpeed;

          if (engine::Input::IsKeyPressed(engine::Key::Space)) {
            cTransform.position += glm::vec3(0, 1, 0) * speed;
          }
        }
      }
    });
}
}
