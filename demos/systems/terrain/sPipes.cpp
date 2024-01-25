#include "systems/terrain/sPipes.h"

namespace demos::systems {

int getTextureAtlasIndex(int index) {
  // 8: 4, 9: 5
  // 4: 8, 5: 9, 6: 10, 7: 11
  // 0: 12, 1: 13, 2: 14, 3: 15
  switch (index) {
  case 0: return 12;
  case 1: return 13;
  case 2: return 14;
  case 3: return 15;
  case 4: return 8;
  case 5: return 9;
  case 6: return 10;
  case 7: return 11;
  case 8: return 4;
  case 9: return 5;
  default: return 0;
  }
}

void PipesSystem::update(entt::registry& registry, const engine::Time& ts) {
  auto& app = engine::Application::Get();
  if (app.isGamePaused()) {
    return;
  }
  const auto& settings = app.getSettings();
  const auto& scene_manager = app.getSceneManager();

  if (settings->activeScene == "Flappy Bird") {
    // state
    static int maxPipes = 4;
    static int scorei1 = 0;
    static int scorei2 = 0;
    static int timei1 = 0;
    static int timei2 = 0;
    if (maxPipes == 0) {
      maxPipes = 4;
    }

    // time
    const auto& time = registry.view<engine::CTime, engine::CUUID>().front();
    const auto& cTime = registry.get<engine::CTime>(time);
    if (cTime.currentSecond >= 59) { // TODO more than 2 digits and real time
      timei2++;
    }
    if (timei2 == 9) { // TODO more than 2 digits and real time
      timei1++;
      timei2 = 0;
    }

    // movement
    registry
      .view<engine::CShaderProgram, engine::CTexture, engine::CTransform,
            engine::CShape, engine::CTag, engine::CName, engine::CUUID>()
      .each([&](entt::entity e, engine::CShaderProgram& cShaderProgram,
                const engine::CTexture& cTexture,
                engine::CTransform& cTransform, engine::CShape& cShape,
                const engine::CTag& cTag, const engine::CName& cName,
                const engine::CUUID& cUUID) {
        if (cTag.tag == "pipe") {
          if (cShaderProgram.isVisible) {
            // int screenW = settings->windowWidth;
            // int screenH = settings->windowHeight;
            // int textureW = cTexture.textures[0]->getWidth();
            // int textureH = cTexture.textures[0]->getHeight();
            float speed = 0.005f;

            // move pipe
            cTransform.position.x -= speed;

            // // after center add point // needs to check how to do this
            // if (cTransform.position.x < 0.5) {
            //   if (scorei2 == 9) { // TODO more than 2 digits
            //     scorei1++;
            //     scorei2 = 0;
            //   } else {
            //     scorei2++;
            //   }
            // }

            // check if pipe is out of screen
            if (cTransform.position.x < -2.f) {
              cShaderProgram.isVisible = false;
              if (scorei2 == 9) { // TODO more than 2 digits
                scorei1++;
                scorei2 = 0;
              } else {
                scorei2++;
              }
            }
          } else {
            if (maxPipes > 0) {
              cShaderProgram.isVisible = true;
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
              maxPipes--;
            }
          }
        }
      });

    // points
    if (scorei1 == 0) {
      entt::entity score2 = scene_manager->getEntity("score2");
      auto& cTextureAtlasScore2 = registry.get<engine::CTextureAtlas>(score2);
      cTextureAtlasScore2.index = getTextureAtlasIndex(scorei2);
    } else {
      entt::entity score1 = scene_manager->getEntity("score1");
      auto& cTextureAtlasScore1 = registry.get<engine::CTextureAtlas>(score1);
      entt::entity score2 = scene_manager->getEntity("score2");
      auto& cTextureAtlasScore2 = registry.get<engine::CTextureAtlas>(score2);
      cTextureAtlasScore1.index = getTextureAtlasIndex(scorei1);
      cTextureAtlasScore2.index = getTextureAtlasIndex(scorei2);
    }

    // time
    if (timei1 == 0) {
      entt::entity time2 = scene_manager->getEntity("time2");
      auto& cTextureAtlasTime2 = registry.get<engine::CTextureAtlas>(time2);
      cTextureAtlasTime2.index = getTextureAtlasIndex(timei2);
    } else {
      entt::entity time1 = scene_manager->getEntity("time1");
      auto& cTextureAtlasTime1 = registry.get<engine::CTextureAtlas>(time1);
      entt::entity time2 = scene_manager->getEntity("time2");
      auto& cTextureAtlasTime2 = registry.get<engine::CTextureAtlas>(time2);
      cTextureAtlasTime1.index = getTextureAtlasIndex(timei1);
      cTextureAtlasTime2.index = getTextureAtlasIndex(timei2);
    }
  }
}
}