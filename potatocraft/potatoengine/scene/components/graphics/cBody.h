#pragma once

#include "potatoengine/pch.h"
#include "potatoengine/scene/components/graphics/cMaterial.h"
#include "potatoengine/scene/components/graphics/cMesh.h"
#include "potatoengine/scene/entity.h"

namespace potatoengine {

struct CBody {
    std::string filepath{};
    std::vector<CMesh> meshes{};
    std::vector<CMaterial> materials{};

    CBody() = default;
    explicit CBody(std::string&& fp) : filepath(std::move(fp)) {}
    explicit CBody(std::string&& fp, std::vector<CMesh>&& m, std::vector<CMaterial>&& ma)
        : filepath(std::move(fp)), meshes(std::move(m)), materials(std::move(ma)) {}

    void print() const {
        CORE_INFO("\t\tfilepath: {0}\n\t\t\t\tmeshes: {1}\n\t\t\t\tmaterials: {2}", filepath, meshes.size(), materials.size());
    }
};
}

template <>
void engine::SceneManager::onComponentAdded(Entity e, CBody& c) {
    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }

    auto model = *manager->get<Model>(c.filepath);
    c.meshes = std::move(model.getMeshes());
    c.materials = std::move(model.getMaterials());
    e.update<CBody>(c);
}