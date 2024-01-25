#include "systems/graphics/sRender.h"

#include <glm/glm.hpp>

namespace demos::systems {

void render(engine::CTexture* cTexture, engine::CTextureAtlas* cTextureAtlas,
            const engine::CSkybox* cSkybox, engine::CMaterial* cMaterial,
            engine::CMesh* cMesh, const engine::CTransform& cTransform,
            const engine::CShaderProgram& cShaderProgram,
            engine::CTexture* cSkyboxTexture,
            const std::unique_ptr<engine::Renderer>& renderer) {
  if (cTexture and cTexture->hasTransparency) {
    engine::RendererAPI::ToggleCulling(false);
  }
  if (cSkybox) {
    engine::RendererAPI::SetDepthLEqual();
  }
  cMesh->bindTextures(renderer->getShaderProgram(cShaderProgram.name), cTexture,
                      cTextureAtlas, cSkyboxTexture, cMaterial);
  renderer->render(cMesh->getVAO(), cTransform.calculate(),
                   cShaderProgram.name);
  cMesh->unbindTextures(cTexture);
  if (cTexture and cTexture->hasTransparency) {
    engine::RendererAPI::ToggleCulling(true);
  }
  if (cSkybox) {
    engine::RendererAPI::SetDepthLess();
  }
}

void RenderSystem::update(entt::registry& registry, const engine::Time& ts) {
  auto& app = engine::Application::Get();
  const auto& renderer = app.getRenderer();

  entt::entity fbo = registry.view<engine::CFBO, engine::CUUID>()
                       .front(); // TODO: support more than one?
  if (fbo not_eq entt::null) {
    engine::CFBO& cfbo = registry.get<engine::CFBO>(fbo);
    const auto& defaultFBO = renderer->getFramebuffers().at(cfbo.fbo);
    defaultFBO->bindToDraw();
    engine::RendererAPI::ToggleDepthTest(true);
  }

  // FBOs are cleared in their own render pass at the end of the scene
  engine::RendererAPI::Clear();
  renderer->resetMetrics();

  entt::entity camera = registry
                          .view<engine::CCamera, engine::CActiveCamera,
                                engine::CTransform, engine::CUUID>()
                          .front();
  ENGINE_ASSERT(camera not_eq entt::null, "No active camera found!");
  engine::CCamera& cCamera = registry.get<engine::CCamera>(camera);
  const engine::CTransform& cCameraTransform =
    registry.get<engine::CTransform>(camera);
  cCamera.calculateView(cCameraTransform.position, cCameraTransform.rotation);
  renderer->beginScene(cCamera.view, cCamera.projection,
                       cCameraTransform.position);

  entt::entity sky = registry.view<engine::CSkybox, engine::CUUID>()
                       .front(); // TODO: support more than one?
  engine::CTexture* cSkyboxTexture;
  if (sky not_eq entt::null) {
    cSkyboxTexture = registry.try_get<engine::CTexture>(sky);
  }

  registry.view<engine::CTransform, engine::CShaderProgram, engine::CUUID>()
    .each([&](entt::entity e, const engine::CTransform& cTransform,
              const engine::CShaderProgram& cShaderProgram,
              const engine::CUUID& cUUID) {
      engine::CTexture* cTexture = registry.try_get<engine::CTexture>(e);
      engine::CTextureAtlas* cTextureAtlas =
        registry.try_get<engine::CTextureAtlas>(e);
      engine::CSkybox* cSkybox = registry.try_get<engine::CSkybox>(e);
      engine::CMaterial* cMaterial = registry.try_get<engine::CMaterial>(e);
      engine::CBody* cBody = registry.try_get<engine::CBody>(e);
      engine::CMesh* cMesh = registry.try_get<engine::CMesh>(e);
      engine::CShape* cShape = registry.try_get<engine::CShape>(e);
      engine::CChunkManager* cChunkManager =
        registry.try_get<engine::CChunkManager>(e);

      if (cShaderProgram.isVisible) {
        if (cMesh) { // TODO objects with one mesh unused
          if (not cTexture) {
            engine::CName* cName = registry.try_get<engine::CName>(e);
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
            engine::CMesh& mesh = cBody->meshes.at(i);
            engine::CMaterial& material = cBody->materials.at(i);
            render(cTexture, cTextureAtlas, cSkybox, &material, &mesh,
                   cTransform, cShaderProgram, cSkyboxTexture, renderer);
          }
        } else if (cShape) { // primitives
          if (not cTexture) {
            engine::CName* cName = registry.try_get<engine::CName>(e);
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
            engine::CName* cName = registry.try_get<engine::CName>(e);
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
          engine::CName* cName = registry.try_get<engine::CName>(e);
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
    engine::CFBO& cfbo = registry.get<engine::CFBO>(fbo);
    const auto& defaultFBO = renderer->getFramebuffers().at(cfbo.fbo);
    defaultFBO->unbind(); // go back to default framebuffer
    engine::RendererAPI::ClearColor();
    engine::RendererAPI::ToggleDepthTest(
      false); // disable depth test so screen-space quad isn't discarded due to
              // depth test.
    engine::CShape& cShape = registry.get<engine::CShape>(fbo);
    cfbo.setupProperties(renderer->getShaderProgram("fbo"));
    const auto& settings = app.getSettings();
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