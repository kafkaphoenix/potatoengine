#pragma once

#include <entt/entt.hpp>

#include "assets/model.h"
#include "core/application.h"
#include "pch.h"
#include "scene/components/graphics/cMaterial.h"
#include "scene/components/graphics/cMesh.h"
#include "utils/mapJsonSerializer.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CBody {
    std::string filepath;
    std::vector<CMesh> meshes;
    std::vector<CMaterial> materials;

    CBody() = default;
    explicit CBody(std::string&& fp) : filepath(std::move(fp)) {}
    explicit CBody(std::string&& fp, std::vector<CMesh>&& m,
                   std::vector<CMaterial>&& ma)
      : filepath(std::move(fp)), meshes(std::move(m)),
        materials(std::move(ma)) {}

    void print() const {
      ENGINE_BACKTRACE(
        "\t\tfilepath: {0}\n\t\t\t\t\t\tmeshes: {1}\n\t\t\t\t\t\tmaterials: {2}",
        filepath, meshes.size(), materials.size());
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["filepath"] = filepath;
      for (uint32_t i = 0; i < meshes.size(); ++i) {
        info["mesh " + std::to_string(i)] = getMeshInfo(i);
      }
      for (uint32_t i = 0; i < materials.size(); ++i) {
        info["material " + std::to_string(i)] = getMaterialInfo(i);
      }

      return info;
    }

    std::string getMeshInfo(uint32_t index) const {
      return MapToJson(meshes.at(index).getInfo());
    }

    std::string getMaterialInfo(uint32_t index) const {
      return MapToJson(materials.at(index).getInfo());
    }

    void setMesh() {
      // TODO rethink if add if not empty here and do it as ctag but creating
      // all the fields
      // TODO support multiple models
      ENGINE_ASSERT(!filepath.empty(), "filepath for model is empty");
      const auto& assets_manager = Application::Get().getAssetsManager();
      auto model = *assets_manager->get<assets::Model>(
        filepath); // We need a copy of the model
      meshes = std::move(model.getMeshes());
      materials = std::move(model.getMaterials());
    }

    void reloadMesh(std::string&& fp) {
      ENGINE_ASSERT(fp != filepath, "filepath for model is the same");
      filepath = std::move(fp);
      setMesh();
    }
};
}

template <>
inline void engine::SceneManager::onComponentAdded(entt::entity e, CBody& c) {
  c.setMesh();

  m_registry.replace<CBody>(e, c);
}