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
#include "scene/components/graphics/cShape.h"
#include "scene/components/graphics/cTexture.h"
#include "scene/components/graphics/cTextureAtlas.h"
#include "scene/components/physics/cTransform.h"
#include "scene/components/world/cChunk.h"
#include "scene/components/world/cChunkManager.h"
#include "scene/components/world/cSkybox.h"

namespace potatoengine {

void render(CTexture* cTexture, CTextureAtlas* cTextureAtlas, CSkybox* cSkybox,
            CMaterial* cMaterial, CMesh* cMesh, const CTransform& cTransform,
            const CShaderProgram& cShaderProgram, CTexture* cSkyboxTexture,
            const std::unique_ptr<Renderer>& renderer) {
  if (cTexture and cTexture->hasTransparency) {
    RendererAPI::ToggleCulling(false);
  }
  if (cSkybox) {
    RendererAPI::SetDepthLEqual();
  }
  cMesh->bindTextures(renderer->getShaderProgram(cShaderProgram.name), cTexture,
                      cTextureAtlas, cSkyboxTexture, cMaterial);
  renderer->render(cMesh->getVAO(), cTransform.calculate(),
                   cShaderProgram.name);
  cMesh->unbindTextures(cTexture);
  if (cTexture and cTexture->hasTransparency) {
    RendererAPI::ToggleCulling(true);
  }
  if (cSkybox) {
    RendererAPI::SetDepthLess();
  }
}

void renderSystem(entt::registry& registry) {
  const auto& renderer = Application::Get().getRenderer();

  entt::entity fbo =
    registry.view<CFBO, CUUID>().front(); // TODO: support more than one?
  if (fbo not_eq entt::null) {
    CFBO& cfbo = registry.get<CFBO>(fbo);
    const auto& defaultFBO = renderer->getFramebuffers().at(cfbo.fbo);
    defaultFBO->bindToDraw();
    RendererAPI::ToggleDepthTest(true);
  }

  // FBOs are cleared in their own render pass at the end of the scene
  RendererAPI::Clear();
  renderer->resetMetrics();

  entt::entity camera =
    registry.view<CCamera, CActiveCamera, CTransform, CUUID>().front();
  ENGINE_ASSERT(camera not_eq entt::null, "No active camera found!");
  CCamera& cCamera = registry.get<CCamera>(camera);
  const CTransform& cCameraTransform = registry.get<CTransform>(camera);
  cCamera.calculateView(cCameraTransform.position, cCameraTransform.rotation);
  renderer->beginScene(cCamera, cCameraTransform);

  entt::entity sky =
    registry.view<CSkybox, CUUID>().front(); // TODO: support more than one?
  CTexture* cSkyboxTexture;
  if (sky not_eq entt::null) {
    cSkyboxTexture = registry.try_get<CTexture>(sky);
  }

  registry.view<CTransform, CShaderProgram, CUUID>().each(
    [&](entt::entity e, const CTransform& cTransform,
        const CShaderProgram& cShaderProgram, const CUUID& cUUID) {
      CTexture* cTexture = registry.try_get<CTexture>(e);
      CTextureAtlas* cTextureAtlas = registry.try_get<CTextureAtlas>(e);
      CSkybox* cSkybox = registry.try_get<CSkybox>(e);
      CMaterial* cMaterial = registry.try_get<CMaterial>(e);
      CBody* cBody = registry.try_get<CBody>(e);
      CMesh* cMesh = registry.try_get<CMesh>(e);
      CShape* cShape = registry.try_get<CShape>(e);
      CChunkManager* cChunkManager = registry.try_get<CChunkManager>(e);

      if (cShaderProgram.isVisible) {
        if (cMesh) { // TODO objects with one mesh unused
          if (not cTexture) {
            CName* cName = registry.try_get<CName>(e);
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
            CName* cName = registry.try_get<CName>(e);
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
            CName* cName = registry.try_get<CName>(e);
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
          CName* cName = registry.try_get<CName>(e);
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
    CFBO& cfbo = registry.get<CFBO>(fbo);
    const auto& defaultFBO = renderer->getFramebuffers().at(cfbo.fbo);
    defaultFBO->unbind(); // go back to default framebuffer
    RendererAPI::ClearColor();
    RendererAPI::ToggleDepthTest(
      false); // disable depth test so screen-space quad isn't discarded due to
              // depth test.
    auto& cShape = registry.get<CShape>(fbo);
    cfbo.setupProperties(renderer->getShaderProgram("fbo"));
    const auto& settings = Application::Get().getSettings();
    if (settings->windowInsideImgui) {
      renderer->renderInsideImGui(cShape.meshes.at(0).getVAO(), cfbo.fbo,
                                  "scene", {0, 0}, {0, 0},
                                  settings->fitToWindow);
    } else {
      renderer->renderFBO(cShape.meshes.at(0).getVAO(), cfbo.fbo);
    }
  }

  renderer->endScene();
}

}