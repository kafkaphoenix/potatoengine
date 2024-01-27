#include "systems/terrain/sPipes.h"

#include "components/config/cPipes.h"

namespace demos::systems {

PipesSystem::~PipesSystem() {
  auto& registry = engine::Application::Get().getSceneManager()->getRegistry();
  for (auto e : registry.view<engine::CTag, engine::CUUID>()) {
    if (registry.get<engine::CTag>(e).tag == "pipe") {
      registry.destroy(e);
    }
  }
}

void PipesSystem::init(entt::registry& registry) {
  const auto& scene_manager = engine::Application::Get().getSceneManager();

  entt::entity gamestate = registry.view<CPipes, engine::CUUID>().front();
  CPipes& pipes_config = registry.get<CPipes>(gamestate);

  entt::entity green_pipe_top =
    scene_manager->createEntity("scene", "pipe", "green_pipe_top");
  registry.get<engine::CTexture>(green_pipe_top)
    .reloadTextures({
      "green_pipe_top",
    });
  registry.get<engine::CShaderProgram>(green_pipe_top).isVisible = false;
  entt::entity green_pipe_bottom =
    scene_manager->createEntity("scene", "pipe", "green_pipe_bottom");
  registry.get<engine::CTexture>(green_pipe_bottom)
    .reloadTextures({
      "green_pipe_bottom",
    });
  registry.get<engine::CShaderProgram>(green_pipe_bottom).isVisible = false;
  entt::entity red_pipe_top =
    scene_manager->createEntity("scene", "pipe", "red_pipe_top");
  registry.get<engine::CTexture>(red_pipe_top)
    .reloadTextures({
      "red_pipe_top",
    });
  registry.get<engine::CShaderProgram>(red_pipe_top).isVisible = false;
  entt::entity red_pipe_bottom =
    scene_manager->createEntity("scene", "pipe", "red_pipe_bottom");
  registry.get<engine::CTexture>(red_pipe_bottom)
    .reloadTextures({
      "red_pipe_bottom",
    });
  registry.get<engine::CShaderProgram>(red_pipe_bottom).isVisible = false;

  pipes_config.pipes = pipes_config.maxPipes;
}

void PipesSystem::update(entt::registry& registry, const engine::Time& ts) {
  if (engine::Application::Get().isGamePaused()) {
    return;
  }

  entt::entity gamestate = registry.view<CPipes, engine::CUUID>().front();
  CPipes& pipes_config = registry.get<CPipes>(gamestate);

  if (pipes_config.pipes == 0) {
    pipes_config.pipes = pipes_config.maxPipes;
  }

  registry
    .view<engine::CShaderProgram, engine::CTransform, engine::CTag,
          engine::CName, engine::CUUID>()
    .each([&](engine::CShaderProgram& cShaderProgram,
              engine::CTransform& cTransform, const engine::CTag& cTag,
              const engine::CName& cName, const engine::CUUID& cUUID) {
      if (cTag.tag == "pipe") {
        if (cShaderProgram.isVisible) {
          float speed = 0.005f; // TODO move to component

          // move pipe
          cTransform.position.x -= speed;

          // check if pipe is out of screen
          if (cTransform.position.x < -2.f) {
            cShaderProgram.isVisible = false;
          }
        } else {
          if (pipes_config.pipes > 0) {
            // randomize y position
            if (cName.name.ends_with("top")) { //
              // -0.6 shortest -0.1 longest
              cTransform.position.y =
                -0.6f + static_cast<float>(rand()) /
                          (static_cast<float>(RAND_MAX / 0.5f));
            } else if (cName.name.ends_with("bottom")) {
              // 1.2 shortest 0.7 longest
              cTransform.position.y =
                1.2f - static_cast<float>(rand()) /
                         (static_cast<float>(RAND_MAX / 0.5f));
            }
            cTransform.position.x = 2.f;
            cShaderProgram.isVisible = true;
            pipes_config.pipes--;
          }
        }
      }
    });
}
}