#include "systems/graphics/sRender.h"

#include <glm/glm.hpp>

namespace demos::systems {

void render(engine::CTexture* cTexture, engine::CTextureAtlas* cTextureAtlas,
            const engine::CSkybox* cSkybox, engine::CMaterial* cMaterial,
            engine::CMesh* cMesh, const engine::CTransform& cTransform,
            const engine::CShaderProgram& cShaderProgram,
            engine::CTexture* cSkyboxTexture, engine::CCollider* cCollider,
            const std::unique_ptr<engine::RenderManager>& render_manager) {
  if (cTexture and cTexture->hasTransparency) {
    engine::RenderAPI::ToggleCulling(false);
  }
  if (cSkybox) {
    engine::RenderAPI::SetDepthLEqual();
  }
  cMesh->bindTextures(render_manager->getShaderProgram(cShaderProgram.name),
                      cTexture, cTextureAtlas, cSkyboxTexture, cMaterial);
  render_manager->render(cMesh->getVAO(), cTransform.calculate(),
                         cShaderProgram.name);
  cMesh->unbindTextures(cTexture);
  if (cTexture and cTexture->hasTransparency) {
    engine::RenderAPI::ToggleCulling(true);
  }
  if (cSkybox) {
    engine::RenderAPI::SetDepthLess();
  }
  if (cCollider and
      engine::Application::Get().getSettingsManager()->displayCollisionBoxes) {
    // TODO fix transparency so I can render this first
    // disabling culling is not working
    const auto& sp = render_manager->getShaderProgram("shape");
    sp->resetActiveUniforms();
    sp->use();
    sp->setFloat("useColor", 1.f);
    sp->setVec4("color", cCollider->color);
    sp->unuse();
    render_manager->render(cCollider->mesh.getVAO(), cTransform.calculate(),
                           "shape");
  }
}

void RenderSystem::update(entt::registry& registry, const engine::Time& ts) {
  auto& app = engine::Application::Get();
  const auto& render_manager = app.getRenderManager();

  entt::entity fbo = registry.view<engine::CFBO, engine::CUUID>()
                       .front(); // TODO: support more than one?
  if (fbo not_eq entt::null) {
    engine::CFBO& cfbo = registry.get<engine::CFBO>(fbo);
    const auto& defaultFBO = render_manager->getFramebuffers().at(cfbo.fbo);
    defaultFBO->bindToDraw();
    engine::RenderAPI::ToggleDepthTest(true);
  }

  // FBOs are cleared in their own render pass at the end of the scene
  engine::RenderAPI::Clear();
  render_manager->resetMetrics();

  entt::entity camera = registry
                          .view<engine::CCamera, engine::CActiveCamera,
                                engine::CTransform, engine::CUUID>()
                          .front();
  ENGINE_ASSERT(camera not_eq entt::null, "No active camera found!");
  engine::CCamera& cCamera = registry.get<engine::CCamera>(camera);
  const engine::CTransform& cCameraTransform =
    registry.get<engine::CTransform>(camera);
  cCamera.calculateView(cCameraTransform.position, cCameraTransform.rotation);
  render_manager->beginScene(cCamera.view, cCamera.projection,
                             cCameraTransform.position);

  entt::entity sky = registry.view<engine::CSkybox, engine::CUUID>()
                       .front(); // TODO: support more than one?
  engine::CTexture* cSkyboxTexture;
  if (sky not_eq entt::null) {
    cSkyboxTexture = registry.try_get<engine::CTexture>(sky);
  }

  if (render_manager->shouldReorder()) {
    registry.sort<engine::CDistanceFromCamera>(
      [](const engine::CDistanceFromCamera& lhs,
         const engine::CDistanceFromCamera& rhs) {
        return lhs.distance < rhs.distance;
      });
    registry.sort<engine::CUUID, engine::CDistanceFromCamera>();
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
      engine::CCollider* cCollider = registry.try_get<engine::CCollider>(e);

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
                 cShaderProgram, cSkyboxTexture, cCollider, render_manager);
        } else if (cBody) { // models
          for (size_t i = 0; i < cBody->meshes.size(); ++i) {
            engine::CMesh& mesh = cBody->meshes.at(i);
            engine::CMaterial& material = cBody->materials.at(i);
            render(cTexture, cTextureAtlas, cSkybox, &material, &mesh,
                   cTransform, cShaderProgram, cSkyboxTexture, cCollider,
                   render_manager);
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
                   cTransform, cShaderProgram, cSkyboxTexture, cCollider,
                   render_manager);
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
                   cSkyboxTexture, cCollider, render_manager);
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
    const auto& defaultFBO = render_manager->getFramebuffers().at(cfbo.fbo);
    defaultFBO->unbind(); // go back to default framebuffer
    engine::RenderAPI::ClearColor();
    engine::RenderAPI::ToggleDepthTest(
      false); // disable depth test so screen-space quad isn't discarded due to
              // depth test.
    engine::CShape& cShape = registry.get<engine::CShape>(fbo);
    cfbo.setupProperties(render_manager->getShaderProgram("fbo"));
    const auto& settings_manager = app.getSettingsManager();
    if (settings_manager->windowInsideImgui) {
      render_manager->renderInsideImGui(cShape.meshes.at(0).getVAO(), cfbo.fbo,
                                        "scene", {0, 0}, {0, 0},
                                        settings_manager->fitToWindow);
    } else {
      render_manager->renderFBO(cShape.meshes.at(0).getVAO(), cfbo.fbo);
    }
  }

  render_manager->endScene();
}

}