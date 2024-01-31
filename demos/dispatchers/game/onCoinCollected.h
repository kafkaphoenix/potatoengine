#pragma once

#include <entt/entt.hpp>

#include "components/meta/cScore.h"
#include "engineAPI.h"

namespace demos::dispatchers {

inline void onCoinCollected(entt::registry& registry) {
  entt::entity gamestate = registry.view<CScore, engine::CUUID>().front();
  CScore& score = registry.get<CScore>(gamestate);
  int new_score = ++score.score;
  registry.view<engine::CTextureAtlas, engine::CName, engine::CUUID>().each(
    [&](engine::CTextureAtlas& cTextureAtlas,
        const engine::CName& cName, const engine::CUUID& cUUID) {
      if (cName.name == "score_unidades") {
        cTextureAtlas.index = new_score % 10;
      } else if (cName.name == "score_decenas") {
        cTextureAtlas.index = (new_score % 100) / 10;
      } else if (cName.name == "score_centenas") {
        cTextureAtlas.index = new_score / 100;
      }
    });
}
}