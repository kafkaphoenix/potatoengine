#include "systems/meta/sScore.h"

#include "components/meta/cScore.h"

namespace demos::systems {

ScoreSystem::~ScoreSystem() {
  auto& registry = engine::Application::Get().getSceneManager()->getRegistry();
  for (auto e : registry.view<engine::CTag, engine::CUUID>()) {
    if (registry.get<engine::CTag>(e).tag == "score") {
      registry.destroy(e);
    }
  }
}

void ScoreSystem::init(entt::registry& registry) {
  const auto& scene_manager = engine::Application::Get().getSceneManager();

  entt::entity unidades =
    scene_manager->createEntity("scene", "numbers", "score_unidades", "score");
  entt::entity decenas =
    scene_manager->createEntity("scene", "numbers", "score_decenas", "score");
  entt::entity centenas =
    scene_manager->createEntity("scene", "numbers", "score_centenas", "score");

  registry.get<engine::CTransform>(unidades).position = {1.6f, 0.8f, 0.f};
  registry.get<engine::CTransform>(decenas).position = {1.45f, 0.8f, 0.f};
  registry.get<engine::CTransform>(centenas).position = {1.3f, 0.8f, 0.f};

  entt::entity gamestate = registry.view<CScore, engine::CUUID>().front();
  int score = registry.get<CScore>(gamestate).score;

  registry.get<engine::CTextureAtlas>(centenas).index = score / 100;
  registry.get<engine::CTextureAtlas>(decenas).index = (score % 100) / 10;
  registry.get<engine::CTextureAtlas>(unidades).index = score % 10;

  registry.get<engine::CShaderProgram>(unidades).isVisible = true;
  registry.get<engine::CShaderProgram>(decenas).isVisible = true;
  registry.get<engine::CShaderProgram>(centenas).isVisible = true;
}

}