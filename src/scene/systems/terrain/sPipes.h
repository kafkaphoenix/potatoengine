#pragma once

#include <entt/entt.hpp>

#include "core/application.h"
#include "scene/components/graphics/cShaderProgram.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/common/cUUID.h"

namespace potatoengine {

void pipesSystem(entt::registry& registry) {
  auto& app = Application::Get();
  const auto& settings = app.getSettings();
  const auto& scene_manager = app.getSceneManager();

  if (settings->activeScene == "Flappy Bird") {
    auto pipe = scene_manager->getEntity("green_pipe_top");
    auto& ptransform = registry.get<engine::CTransform>(pipe);
    ptransform.position = {1.5f, -0.1f, 0.0f};
    auto& ptexture = registry.get<engine::CShaderProgram>(pipe); // rename
    ptexture.isVisible = true;

    auto pipe2 = scene_manager->getEntity("green_pipe_bottom");
    auto& ptransform2 = registry.get<engine::CTransform>(pipe2);
    ptransform2.position = {1.f, 1.f, 0.0f};
    auto& ptexture2 = registry.get<engine::CShaderProgram>(pipe2);
    ptexture2.isVisible = true;

    auto pipe3 = scene_manager->getEntity("red_pipe_top");
    auto& ptransform3 = registry.get<engine::CTransform>(pipe3);
    ptransform3.position = {-0.5f, -0.1f, 0.0f};
    auto& ptexture3 = registry.get<engine::CShaderProgram>(pipe3);
    ptexture3.isVisible = true;

    auto pipe4 = scene_manager->getEntity("red_pipe_bottom");
    auto& ptransform4 = registry.get<engine::CTransform>(pipe4);
    ptransform4.position = {-1.5f, 0.75f, 0.0f};
    auto& ptexture4 = registry.get<engine::CShaderProgram>(pipe4);
    ptexture4.isVisible = true;
  }
}
}