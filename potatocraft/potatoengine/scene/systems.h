#pragma once

#include <glm/glm.hpp>

#include "potatoengine/core/time.h"
#include "potatoengine/pch.h"
#include "potatoengine/renderer/renderer.h"
#include "potatoengine/scene/components.h"

namespace potatoengine {

void deleteSystem(entt::registry& reg) {
    auto to_destroy = reg.view<Deleted>();
    reg.destroy(std::begin(to_destroy), std::end(to_destroy));
}

void printEntities(entt::registry& reg) {
    printf("================================\n");

    auto entities = reg.view<Transform, UUIDComponent>();
    for (auto entity : entities) {
        auto [transform, uuid] = entities.get<Transform, UUIDComponent>(entity);
        printf("pos (%d) (%d): %.3f, %.3f, %.3f\n", entt::to_integral(entity), uuid.uuid, transform.pos.x, transform.pos.y, transform.pos.z);
    }
}

void movementSystem(entt::registry& reg, Time ts) {
    auto entities = reg.view<Transform, Body, UUIDComponent>();
    for (auto entity : entities) {
        auto [transform, body] = entities.get<Transform, Body>(entity);
        transform.rot = glm::rotate(transform.rot, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}

void renderSystem(entt::registry& reg, std::weak_ptr<Renderer> r) {
    const auto& renderer = r.lock();
    if (not renderer) {
        throw std::runtime_error("Renderer is null!");
    }
    auto entities = reg.view<Transform, Body, UUIDComponent>();
    for (auto entity : entities) {
        auto [transform, body] = reg.get<Transform, Body>(entity);
        auto textureOpts = reg.try_get<TextureOpts>(entity);
        auto textureAtlas = reg.try_get<TextureAtlas>(entity);

        for (auto& mesh : body.meshes) {
            if (textureOpts) {
                if (textureOpts->hasTransparency) {
                    RendererAPI::EnableCulling(false);
                }
            }
            mesh.bindTextures(renderer->getShaderProgram(mesh.getShaderProgram()), textureAtlas);
            renderer->render(mesh.getVAO(), transform.get());
            if (textureOpts) {
                if (textureOpts->hasTransparency) {
                    RendererAPI::EnableCulling(true);
                }
            }
        }
    }
    // auto group = registry.group<TransformComponent>(entt::get<AsteroidComponent>);
    // group.each([](auto& transform, auto const &asteroid) {
    //     transform.rotation += vec3{ 0.01f, 0.03f, 0.01f };
    // });
    // r.view<TransformComponent, ModelComponent>().each([&](entt::entity, TransformComponent &, ModelComponent &){
    // "do stuff";
    // });
    // http://vasir.net/blog/game-development/how-to-build-entity-component-system-in-javascript
    // https://www.reddit.com/r/gamedev/comments/nmye1y/help_with_the_systems_part_of_entity_component/
    // https://austinmorlan.com/posts/entity_component_system/#the-system-manager
    // https://jsforgames.com/collision-detection/ collysion system
    // https://www.reddit.com/r/gamedev/comments/2zg5x8/what_is_the_best_way_to_implement_a_system/
    // auto group = mRegistry.group<TransformComponent>( entt::get<MeshComponent> );
    // for ( auto e : group )
    // {
    //     auto [transform, mesh] = group.get<TransformComponent, MeshComponent>( e );

    //     mesh.mMatrix = transform.GetTransform();
    // }
}

void updateSystems(entt::registry& reg, std::weak_ptr<Renderer> r, Time ts) {
    deleteSystem(reg);
    renderSystem(reg, r);
    movementSystem(reg, ts);
    // printEntities(reg);
}
}