#pragma once

#include <glm/glm.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/renderer/renderer.h"
#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/graphics/cBody.h"
#include "potatoengine/scene/components/graphics/cMaterial.h"
#include "potatoengine/scene/components/graphics/cMesh.h"
#include "potatoengine/scene/components/graphics/cShaderProgram.h"
#include "potatoengine/scene/components/graphics/cTexture.h"
#include "potatoengine/scene/components/graphics/cTextureAtlas.h"
#include "potatoengine/scene/components/physics/cTransform.h"
#include "potatoengine/scene/components/world/cSkybox.h"

namespace potatoengine {

void renderSystem(entt::registry& reg, std::weak_ptr<Renderer> r) {
    const auto& renderer = r.lock();
    if (not renderer) {
        throw std::runtime_error("Renderer is null!");
    }

    entt::entity sky = reg.view<CSkybox, CUUID>().front();
    CTexture* cSkyboxTexture;
    if (sky not_eq entt::null) {
        cSkyboxTexture = reg.try_get<CTexture>(sky);
    }

    reg.view<CTransform, CBody, CShaderProgram, CUUID>().each([&](entt::entity e, const CTransform& cTransform, CBody& cBody, const CShaderProgram& cShaderProgram, const CUUID& cUUID) {
        CTexture* cTexture = reg.try_get<CTexture>(e);
        CTextureAtlas* cTextureAtlas = reg.try_get<CTextureAtlas>(e);
        CSkybox* cSkybox = reg.try_get<CSkybox>(e);
        CMaterial* cMaterial = reg.try_get<CMaterial>(e);

        for (CMesh& mesh : cBody.meshes) {
            if (cTexture and cTexture->hasTransparency) {
                RendererAPI::SetCulling(false);
            }
            if (cSkybox) {
                RendererAPI::SetDepthLEqual();
            }
            mesh.bindTextures(
                renderer->get(cShaderProgram.shaderProgram),
                cTexture,
                cTextureAtlas,
                cSkyboxTexture,
                renderer->getCameraPosition(),
                cMaterial);
            renderer->render(mesh.getVAO(), cTransform.calculate(), cShaderProgram.shaderProgram);
            mesh.unbindTextures(cTexture);
            if (cTexture and cTexture->hasTransparency) {
                RendererAPI::SetCulling(true);
            }
            if (cSkybox) {
                RendererAPI::SetDepthLess();
            }
        }
    });
}
}