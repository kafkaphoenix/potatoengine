#pragma once

#include <glm/glm.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/renderer/renderer.h"
#include "potatoengine/renderer/rendererAPI.h"
#include "potatoengine/scene/components/camera/cActiveCamera.h"
#include "potatoengine/scene/components/camera/cCamera.h"
#include "potatoengine/scene/components/common/cName.h"
#include "potatoengine/scene/components/common/cUUID.h"
#include "potatoengine/scene/components/graphics/cBody.h"
#include "potatoengine/scene/components/graphics/cFBO.h"
#include "potatoengine/scene/components/graphics/cMaterial.h"
#include "potatoengine/scene/components/graphics/cMesh.h"
#include "potatoengine/scene/components/graphics/cShaderProgram.h"
#include "potatoengine/scene/components/graphics/cTexture.h"
#include "potatoengine/scene/components/graphics/cTextureAtlas.h"
#include "potatoengine/scene/components/physics/cTransform.h"
#include "potatoengine/scene/components/utils/cShape.h"
#include "potatoengine/scene/components/world/cChunk.h"
#include "potatoengine/scene/components/world/cChunkManager.h"
#include "potatoengine/scene/components/world/cSkybox.h"

namespace potatoengine {

void render(CTexture* cTexture, CTextureAtlas* cTextureAtlas, CSkybox* cSkybox, CMaterial* cMaterial, CMesh* cMesh, const CTransform& cTransform, const CShaderProgram& cShaderProgram, CTexture* cSkyboxTexture, const std::shared_ptr<Renderer>& r) {
    if (cTexture and cTexture->hasTransparency) {
        RendererAPI::SetCulling(false);
    }
    if (cSkybox) {
        RendererAPI::SetDepthLEqual();
    }
    cMesh->bindTextures(
        r->get(cShaderProgram.name),
        cTexture,
        cTextureAtlas,
        cSkyboxTexture,
        cMaterial);
    r->render(cMesh->getVAO(), cTransform.calculate(), cShaderProgram.name);
    cMesh->unbindTextures(cTexture);
    if (cTexture and cTexture->hasTransparency) {
        RendererAPI::SetCulling(true);
    }
    if (cSkybox) {
        RendererAPI::SetDepthLess();
    }
}

void renderSystem(entt::registry& reg, std::weak_ptr<Renderer> r) {
    const auto& renderer = r.lock();
    if (not renderer) {
        throw std::runtime_error("Renderer is null!");
    }

    entt::entity camera = reg.view<CCamera, CActiveCamera, CUUID>().front();
    if (camera == entt::null) {
        throw std::runtime_error("No camera found!");
    }
    const CCamera& cCamera = reg.get<CCamera>(camera);
    const CTransform& cCameraTransform = reg.get<CTransform>(camera);
    renderer->beginScene(cCamera, cCameraTransform);

    entt::entity sky = reg.view<CSkybox, CUUID>().front();  // TODO: support more than one?
    CTexture* cSkyboxTexture;
    if (sky not_eq entt::null) {
        cSkyboxTexture = reg.try_get<CTexture>(sky);
    }

    entt::entity fbo = reg.view<CFBO, CUUID>().front();  // TODO: support more than one?
    if (fbo not_eq entt::null) {
        CFBO& cfbo = reg.get<CFBO>(fbo);
        const auto& defaultFBO = renderer->getFramebuffers().at(cfbo.fbo);
        defaultFBO->bindToDraw();
        RendererAPI::SetDepthTest(true);
    }
    RendererAPI::Clear();

    reg.view<CTransform, CShaderProgram, CUUID>().each([&](entt::entity e, const CTransform& cTransform, const CShaderProgram& cShaderProgram, const CUUID& cUUID) {
        CTexture* cTexture = reg.try_get<CTexture>(e);
        CTextureAtlas* cTextureAtlas = reg.try_get<CTextureAtlas>(e);
        CSkybox* cSkybox = reg.try_get<CSkybox>(e);
        CMaterial* cMaterial = reg.try_get<CMaterial>(e);
        CBody* cBody = reg.try_get<CBody>(e);
        CMesh* cMesh = reg.try_get<CMesh>(e);
        CShape* cShape = reg.try_get<CShape>(e);
        CChunkManager* cChunkManager = reg.try_get<CChunkManager>(e);

        if (cShaderProgram.isVisible) {
            if (cMesh) {  // objects
                if (not cTexture) {
                    CName* cName = reg.try_get<CName>(e);
                    if (cName) {
                        throw std::runtime_error("No texture found for entity " + std::to_string(cUUID.uuid) + " " + cName->name);
                    } else {
                        throw std::runtime_error("No texture found for entity " + std::to_string(cUUID.uuid));
                    }
                }

                render(cTexture, cTextureAtlas, cSkybox, cMaterial, cMesh, cTransform, cShaderProgram, cSkyboxTexture, renderer);
            } else if (cBody) {  // models
                for (size_t i = 0; i < cBody->meshes.size(); ++i) {
                    CMesh& mesh = cBody->meshes.at(i);
                    CMaterial& material = cBody->materials.at(i);
                    render(cTexture, cTextureAtlas, cSkybox, &material, &mesh, cTransform, cShaderProgram, cSkyboxTexture, renderer);
                }
            } else if (cShape) {  // primitives
                if (not cTexture) {
                    CName* cName = reg.try_get<CName>(e);
                    if (cName) {
                        throw std::runtime_error("No texture found for entity " + std::to_string(cUUID.uuid) + " " + cName->name);
                    } else {
                        throw std::runtime_error("No texture found for entity " + std::to_string(cUUID.uuid));
                    }
                }

                for (auto& mesh : cShape->meshes) {
                    render(cTexture, cTextureAtlas, cSkybox, cMaterial, &mesh, cTransform, cShaderProgram, cSkyboxTexture, renderer);
                }
            } else if (cChunkManager) {  // terrain
                if (not cTexture) {
                    CName* cName = reg.try_get<CName>(e);
                    if (cName) {
                        throw std::runtime_error("No texture found for entity " + std::to_string(cUUID.uuid) + " " + cName->name);
                    } else {
                        throw std::runtime_error("No texture found for entity " + std::to_string(cUUID.uuid));
                    }
                }

                for (auto& [position, chunk] : cChunkManager->chunks) {
                    render(cTexture, cTextureAtlas, cSkybox, cMaterial, &chunk.terrainMesh, chunk.transform, cShaderProgram, cSkyboxTexture, renderer);
                }
            } else {
                CName* cName = reg.try_get<CName>(e);
                if (cName) {
                    throw std::runtime_error("No mesh found for entity " + std::to_string(cUUID.uuid) + " " + cName->name);
                } else {
                    throw std::runtime_error("No mesh found for entity " + cUUID.uuid);
                }
            }
        }
    });

    if (fbo not_eq entt::null) {
        CFBO& cfbo = reg.get<CFBO>(fbo);
        const auto& defaultFBO = renderer->getFramebuffers().at(cfbo.fbo);
        defaultFBO->unbind();  // go back to default framebuffer
        RendererAPI::ClearColor();
        engine::RendererAPI::SetDepthTest(false);  // disable depth test so screen-space quad isn't discarded due to depth test.
        auto& cShape = reg.get<CShape>(fbo);
        cfbo.setupProperties(renderer->get("fbo"));
        renderer->renderFBO(cShape.meshes.at(0).getVAO(), cfbo.fbo);
    }

    renderer->endScene();
}

}