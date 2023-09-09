#pragma once

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <random>

#include "potatoengine/core/time.h"
#include "potatoengine/pch.h"
#include "potatoengine/renderer/renderer.h"
#include "potatoengine/scene/components.h"

namespace potatoengine {

void deleteSystem(entt::registry& r) {
    auto to_destroy = r.view<Deleted, UUIDComponent>();
    r.destroy(std::begin(to_destroy), std::end(to_destroy));
}

void printEntities(entt::registry& r) {
    CORE_INFO("================================");
    r.view<Transform, UUIDComponent>().each([&](entt::entity e, const Transform& cTransform, const UUIDComponent& cUUID) {
        CORE_INFO("position ({0}) ({1}): {2}, {3}, {4}", entt::to_integral(e), cUUID.uuid, cTransform.position.x, cTransform.position.y, cTransform.position.z);
    });
}

void movementSystem(entt::registry& r, Time ts) {
    r.view<Transform, RigidBody, UUIDComponent>().each([&](entt::entity e, Transform& cTransform, const RigidBody& cRigidBody, const UUIDComponent& cUUID) {
        Skybox* cSkybox = r.try_get<Skybox>(e);

        if (cRigidBody.isKinematic) {
            // TODO sun movement system
            // TODO real movement system
            cTransform.rotation = glm::angleAxis(glm::radians(1.f), glm::vec3(0.f, 1.f, 0.f)) * cTransform.rotation;
            // glm quatLookAt allows you to construct quaternion from a unit vector of a direction you want to rotate
        }

        if (cSkybox) {
            const TimeComponent& cTime = r.get<TimeComponent>(e);
            float rotation = 0.f;
            if (cSkybox->rotationSpeed > 0.f) {
                rotation = cSkybox->rotationSpeed;
            } else {
                // to avoid rotating fps frames per second instead of one we need to divide by fps
                float rotationAnglePerSecond = (360.f / (cTime.dayLength * 3600.f)) / cTime.fps;
                rotation = rotationAnglePerSecond * cTime.acceleration;
            }
            cTransform.rotation = glm::angleAxis(glm::radians(rotation), glm::vec3(0.f, 1.f, 0.f)) * cTransform.rotation;
        }
    });
}

void renderSystem(entt::registry& reg, std::weak_ptr<Renderer> r) {
    const auto& renderer = r.lock();
    if (not renderer) {
        throw std::runtime_error("Renderer is null!");
    }

    entt::entity sky = reg.view<Skybox, UUIDComponent>().front();
    Skybox* _cSkybox;
    TextureComponent* _cSkyboxTexture;
    if (sky not_eq entt::null) {
        _cSkybox = reg.try_get<Skybox>(sky);
        _cSkyboxTexture = reg.try_get<TextureComponent>(sky);
    }
    entt::entity light = reg.view<Light, Transform, UUIDComponent>().front();
    glm::vec3 lightPosition{};
    glm::vec4 lightColor{};
    if (light not_eq entt::null) {
        const Transform& _cTransform = reg.get<Transform>(light);
        lightPosition = _cTransform.position;
        lightColor = reg.get<Light>(light).color;
    }
    reg.view<Transform, Body, ShaderProgramComponent, UUIDComponent>().each([&](entt::entity e, const Transform& cTransform, Body& cBody, const ShaderProgramComponent& cShaderProgram, const UUIDComponent& cUUID) {
        TextureComponent* cTexture = reg.try_get<TextureComponent>(e);
        TextureAtlas* cTextureAtlas = reg.try_get<TextureAtlas>(e);
        Skybox* cSkybox = reg.try_get<Skybox>(e);
        Material* cMaterial = reg.try_get<Material>(e);

        for (Mesh& mesh : cBody.meshes) {
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
                cSkybox,
                _cSkybox,
                _cSkyboxTexture,
                lightPosition,
                lightColor,
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

void timeSystem(entt::registry& reg, Time ts) {
    reg.view<TimeComponent>().each([&](entt::entity e, TimeComponent& cTime) {
        // We call it fps frames per second instead of 1 but the other functions divide by fps so it's ok
        float timeScale = 86400.f / (cTime.dayLength * 3600.f);
        cTime.seconds += ts.getSeconds() * timeScale * cTime.acceleration;
        if (cTime.seconds >= 86400) {
            cTime.seconds = 0;
        }
        // dividing by fps because we call this function fps frames per second instead of 1
        // but at the same time it helps us to see the time correctly per frame so the logic works for 24 hours
        // TODO revisit probably will break with different fps
        cTime.currentHour = static_cast<int>(cTime.seconds / cTime.fps) / cTime.fps;
        cTime.currentMinute = static_cast<int>(cTime.seconds / cTime.fps) % cTime.fps;
        cTime.currentSecond = static_cast<int>(cTime.seconds) % cTime.fps;
    });
}

void skyboxSystem(entt::registry& reg) {
    reg.view<Skybox, TimeComponent, TextureComponent, UUIDComponent>().each([&](entt::entity e, const Skybox& cSkybox, const TimeComponent& cTime, TextureComponent& cTexture, const UUIDComponent& cUUID) {
        if (cTexture.useBlending) {
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
        }
    });
}

void updateSystems(entt::registry& reg, std::weak_ptr<Renderer> r, const Time& ts) {
    timeSystem(reg, ts);
    deleteSystem(reg);
    movementSystem(reg, ts);
    skyboxSystem(reg);
    renderSystem(reg, r);
    // printEntities(reg);
}
}