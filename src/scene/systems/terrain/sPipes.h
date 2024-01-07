#pragma once

#include <entt/entt.hpp>

#include "core/application.h"
#include "scene/components/graphics/cShaderProgram.h"
#include "scene/components/physics/cRigidBody.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/common/cUUID.h"

namespace potatoengine {

void pipesSystem() {
  auto& app = Application::Get();
  const auto& settings = app.getSettings();
  const auto& scene_manager = app.getSceneManager();

  if (settings->activeScene == "Flappy Bird") {
    auto pipe = scene_manager->getEntity("green_pipe_top");
    auto& ptransform = pipe.get<engine::CTransform>();
    ptransform.position = {1.5f, -0.1f, 0.0f};
    auto& ptexture = pipe.get<engine::CShaderProgram>();
    ptexture.isVisible = true;

    auto pipe2 = scene_manager->getEntity("green_pipe_bottom");
    auto& ptransform2 = pipe2.get<engine::CTransform>();
    ptransform2.position = {1.f, 1.f, 0.0f};
    auto& ptexture2 = pipe2.get<engine::CShaderProgram>();
    ptexture2.isVisible = true;

    auto pipe3 = scene_manager->getEntity("red_pipe_top");
    auto& ptransform3 = pipe3.get<engine::CTransform>();
    ptransform3.position = {-0.5f, -0.1f, 0.0f};
    auto& ptexture3 = pipe3.get<engine::CShaderProgram>();
    ptexture3.isVisible = true;

    auto pipe4 = scene_manager->getEntity("red_pipe_bottom");
    auto& ptransform4 = pipe4.get<engine::CTransform>();
    ptransform4.position = {-1.5f, 0.75f, 0.0f};
    auto& ptexture4 = pipe4.get<engine::CShaderProgram>();
    ptexture4.isVisible = true;
  }
}
}