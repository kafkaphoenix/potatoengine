#pragma once

#include <entt/entt.hpp>

#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/graphics/cTexture.h"
#include "potatoengine/scene/components/world/cSkybox.h"
#include "potatoengine/scene/components/world/cTime.h"

namespace potatoengine {

void skyboxSystem(entt::registry& reg) {
    using namespace entt::literals;
    
    reg.view<CSkybox, CTime, CTexture, CUUID>().each([&](const CSkybox& cSkybox, const CTime& cTime, CTexture& cTexture, const CUUID& cUUID) {
        if (cTexture.drawMode == CTexture::DrawMode::TEXTURES_BLEND) {
            float blendFactor = 0.f;
            if (cTime.currentHour >= cTime.nightStart and cTime.currentHour < cTime.dayTransitionStart) {
                blendFactor = 0.f;
            } else if (cTime.currentHour >= cTime.dayTransitionStart and cTime.currentHour < cTime.dayStart) {
                blendFactor = (cTime.currentMinute + (cTime.currentHour - cTime.dayTransitionStart) * cTime.fps) / 120.f;
            } else if (cTime.currentHour >= cTime.dayStart and cTime.currentHour < cTime.nightTransitionStart) {
                blendFactor = 1.f;
            } else if (cTime.currentHour >= cTime.nightTransitionStart and cTime.currentHour < cTime.nightStart) {
                blendFactor = 1.f - ((cTime.currentMinute + (cTime.currentHour - cTime.nightTransitionStart) * cTime.fps) / 120.f);
            }
            cTexture.blendFactor = blendFactor;
            entt::monostate<"skyBlendFactor"_hs>{} = blendFactor;
            entt::monostate<"useSkyBlending"_hs>{} = 1.f;
        } else {
            entt::monostate<"useSkyBlending"_hs>{} = 0.f;
        }

        if (cSkybox.useFog) {
            entt::monostate<"useFog"_hs>{} = 1.f;
            entt::monostate<"fogDensity"_hs>{} = cSkybox.fogDensity;
            entt::monostate<"fogGradient"_hs>{} = cSkybox.fogGradient;
            entt::monostate<"fogColor"_hs>{} = cSkybox.fogColor;
        } else {
            entt::monostate<"useFog"_hs>{} = 0.f;
        }
    });
}
}