#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/application.h"
#include "pch.h"
#include "renderer/renderer.h"
#include "renderer/rendererAPI.h"
#include "scene/components/camera/cActiveCamera.h"
#include "scene/components/camera/cCamera.h"
#include "scene/components/common/cName.h"
#include "scene/components/common/cUUID.h"
#include "scene/components/graphics/cBody.h"
#include "scene/components/graphics/cFBO.h"
#include "scene/components/graphics/cMaterial.h"
#include "scene/components/graphics/cMesh.h"
#include "scene/components/graphics/cShaderProgram.h"
#include "scene/components/graphics/cTexture.h"
#include "scene/components/graphics/cTextureAtlas.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/utils/cShape.h"
#include "scene/components/world/cChunk.h"
#include "scene/components/world/cChunkManager.h"
#include "scene/components/world/cSkybox.h"

namespace potatoengine {

void render(CTexture* cTexture, CTextureAtlas* cTextureAtlas, CSkybox* cSkybox,
            CMaterial* cMaterial, CMesh* cMesh, const CTransform& cTransform,
            const CShaderProgram& cShaderProgram, CTexture* cSkyboxTexture,
            const std::shared_ptr<Renderer>& r) {
  if (cTexture and cTexture->hasTransparency) {
    RendererAPI::ToggleCulling(false);
  }
  if (cSkybox) {
    RendererAPI::SetDepthLEqual();
  }
  cMesh->bindTextures(r->get(cShaderProgram.name), cTexture, cTextureAtlas,
                      cSkyboxTexture, cMaterial);
  r->render(cMesh->getVAO(), cTransform.calculate(), cShaderProgram.name);
  cMesh->unbindTextures(cTexture);
  if (cTexture and cTexture->hasTransparency) {
    RendererAPI::ToggleCulling(true);
  }
  if (cSkybox) {
    RendererAPI::SetDepthLess();
  }
}

void renderSystem(entt::registry& reg, std::weak_ptr<Renderer> r) {
  const auto& renderer = r.lock();
  ENGINE_ASSERT(renderer, "Renderer is null!")

  entt::entity fbo =
    reg.view<CFBO, CUUID>().front(); // TODO: support more than one?
  if (fbo not_eq entt::null) {
    CFBO& cfbo = reg.get<CFBO>(fbo);
    const auto& defaultFBO = renderer->getFramebuffers().at(cfbo.fbo);
    defaultFBO->bindToDraw();
    RendererAPI::ToggleDepthTest(true);
  }

  // FBOs are cleared in their own render pass at the end of the scene
  RendererAPI::Clear();
  renderer->resetMetrics();

  entt::entity camera =
    reg.view<CCamera, CActiveCamera, CTransform, CUUID>().front();
  ENGINE_ASSERT(camera not_eq entt::null, "No camera found!");
  const CCamera& cCamera = reg.get<CCamera>(camera);
  const CTransform& cCameraTransform = reg.get<CTransform>(camera);
  renderer->beginScene(cCamera, cCameraTransform);

  entt::entity sky =
    reg.view<CSkybox, CUUID>().front(); // TODO: support more than one?
  CTexture* cSkyboxTexture;
  if (sky not_eq entt::null) {
    cSkyboxTexture = reg.try_get<CTexture>(sky);
  }

  reg.view<CTransform, CShaderProgram, CUUID>().each(
    [&](entt::entity e, const CTransform& cTransform,
        const CShaderProgram& cShaderProgram, const CUUID& cUUID) {
      CTexture* cTexture = reg.try_get<CTexture>(e);
      CTextureAtlas* cTextureAtlas = reg.try_get<CTextureAtlas>(e);
      CSkybox* cSkybox = reg.try_get<CSkybox>(e);
      CMaterial* cMaterial = reg.try_get<CMaterial>(e);
      CBody* cBody = reg.try_get<CBody>(e);
      CMesh* cMesh = reg.try_get<CMesh>(e);
      CShape* cShape = reg.try_get<CShape>(e);
      CChunkManager* cChunkManager = reg.try_get<CChunkManager>(e);

      if (cShaderProgram.isVisible) {
        if (cMesh) { // TODO objects with one mesh unused
          if (not cTexture) {
            CName* cName = reg.try_get<CName>(e);
            if (cName) {
              ENGINE_ASSERT(false, "No texture found for entity {} {}",
                            cUUID.uuid, cName->name);
            } else {
              ENGINE_ASSERT(false, "No texture found for entity {}",
                            cUUID.uuid);
            }
          }

          render(cTexture, cTextureAtlas, cSkybox, cMaterial, cMesh, cTransform,
                 cShaderProgram, cSkyboxTexture, renderer);
        } else if (cBody) { // models
          for (size_t i = 0; i < cBody->meshes.size(); ++i) {
            CMesh& mesh = cBody->meshes.at(i);
            CMaterial& material = cBody->materials.at(i);
            render(cTexture, cTextureAtlas, cSkybox, &material, &mesh,
                   cTransform, cShaderProgram, cSkyboxTexture, renderer);
          }
        } else if (cShape) { // primitives
          if (not cTexture) {
            CName* cName = reg.try_get<CName>(e);
            if (cName) {
              ENGINE_ASSERT(false, "No texture found for entity {} {}",
                            cUUID.uuid, cName->name);
            } else {
              ENGINE_ASSERT(false, "No texture found for entity {}",
                            cUUID.uuid);
            }
          }

          for (auto& mesh : cShape->meshes) {
            render(cTexture, cTextureAtlas, cSkybox, cMaterial, &mesh,
                   cTransform, cShaderProgram, cSkyboxTexture, renderer);
          }
        } else if (cChunkManager) { // terrain
          if (not cTexture) {
            CName* cName = reg.try_get<CName>(e);
            if (cName) {
              ENGINE_ASSERT(false, "No texture found for entity {} {}",
                            cUUID.uuid, cName->name);
            } else {
              ENGINE_ASSERT(false, "No texture found for entity {}",
                            cUUID.uuid);
            }
          }

          for (auto& [position, chunk] : cChunkManager->chunks) {
            render(cTexture, cTextureAtlas, cSkybox, cMaterial,
                   &chunk.terrainMesh, chunk.transform, cShaderProgram,
                   cSkyboxTexture, renderer);
          }
        } else {
          CName* cName = reg.try_get<CName>(e);
          if (cName) {
            ENGINE_ASSERT(false, "No mesh found for entity {} {}", cUUID.uuid,
                          cName->name);
          } else {
            ENGINE_ASSERT(false, "No mesh found for entity {}", cUUID.uuid);
          }
        }
      }
    });

  if (fbo not_eq entt::null) {
    CFBO& cfbo = reg.get<CFBO>(fbo);
    const auto& defaultFBO = renderer->getFramebuffers().at(cfbo.fbo);
    defaultFBO->unbind(); // go back to default framebuffer
    RendererAPI::ClearColor();
    RendererAPI::ToggleDepthTest(
      false); // disable depth test so screen-space quad isn't discarded due to
              // depth test.
    auto& cShape = reg.get<CShape>(fbo);
    cfbo.setupProperties(renderer->get("fbo"));
    auto& window = Application::Get().getWindow();
    if (window.isWindowInsideImgui()) {
      renderer->renderInsideImGui(cShape.meshes.at(0).getVAO(), cfbo.fbo,
                                  "scene", {0, 0}, {0, 0});
    } else {
      renderer->renderFBO(cShape.meshes.at(0).getVAO(), cfbo.fbo);
    }
  }

  renderer->endScene();
}

}