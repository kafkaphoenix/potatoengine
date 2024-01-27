#include "scene/entityFactory.h"

#include <entt/core/hashed_string.hpp>
#include <entt/meta/meta.hpp>
#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <nlohmann/json.hpp>

#include "scene/components/core/cDeleted.h"

using json = nlohmann::json;
using namespace entt::literals;

namespace potatoengine {

glm::vec2 vec2FromJson(const json& data) {
  glm::vec2 vec{};
  vec.x = data.at("x").get<float>();
  vec.y = data.at("y").get<float>();
  return vec;
}

glm::vec3 vec3FromJson(const json& data, std::string_view type = "") {
  glm::vec3 vec{};
  vec.x =
    (type == "color") ? data.at("r").get<float>() : data.at("x").get<float>();
  vec.y =
    (type == "color") ? data.at("g").get<float>() : data.at("y").get<float>();
  vec.z =
    (type == "color") ? data.at("b").get<float>() : data.at("z").get<float>();
  return vec;
}

glm::vec4 vec4FromJson(const json& data, std::string_view type = "") {
  glm::vec4 vec{};
  vec.x =
    (type == "color") ? data.at("r").get<float>() : data.at("x").get<float>();
  vec.y =
    (type == "color") ? data.at("g").get<float>() : data.at("y").get<float>();
  vec.z =
    (type == "color") ? data.at("b").get<float>() : data.at("z").get<float>();
  vec.w =
    (type == "color") ? data.at("a").get<float>() : data.at("w").get<float>();
  return vec;
}

glm::quat quatFromJson(const json& data) {
  glm::quat quat{glm::identity<glm::quat>()};
  quat.x = data.at("x").get<float>();
  quat.y = data.at("y").get<float>();
  quat.z = data.at("z").get<float>();
  quat.w = data.at("w").get<float>();
  return quat;
}

void processCTag(entt::entity& e, std::string_view cTag) {
  entt::meta_type cType = entt::resolve(entt::hashed_string{cTag.data()});
  ENGINE_ASSERT(cType, "No component type found for component tag {}", cTag)

  entt::meta_func assignFunc = cType.func("assign"_hs);
  ENGINE_ASSERT(assignFunc, "No assign function found for component tag {}",
                cTag)

  entt::meta_any metaComponent = assignFunc.invoke({}, e);
  entt::meta_func triggerEventFunc = cType.func("onComponentAdded"_hs);
  if (triggerEventFunc) {
    triggerEventFunc.invoke({}, e, metaComponent);
  }
}

void processComponent(entt::entity& e, const std::string& cPrefab,
                      const json& cValue) {
  entt::meta_type cType = entt::resolve(entt::hashed_string{cPrefab.data()});
  ENGINE_ASSERT(cType, "No component type found for component {}", cPrefab)

  entt::meta_func assignFunc = cType.func("assign"_hs);
  ENGINE_ASSERT(assignFunc, "No assign function found for component {}",
                cPrefab)

  entt::meta_any metaComponent;
  if (cValue.is_string()) {
    metaComponent = assignFunc.invoke({}, e, cValue.get<std::string>());
  } else if (cValue.is_number_integer()) {
    metaComponent = assignFunc.invoke({}, e, cValue.get<int>());
  } else if (cValue.is_number_float()) {
    metaComponent = assignFunc.invoke({}, e, cValue.get<float>());
  } else if (cValue.is_boolean()) {
    metaComponent = assignFunc.invoke({}, e, cValue.get<bool>());
  } else if (cValue.is_object() and cValue.contains("x") and
             cValue.contains("y") and cValue.contains("z")) {
    metaComponent = assignFunc.invoke({}, e, vec3FromJson(cValue));
  } else if (cValue.is_object()) {
    metaComponent = assignFunc.invoke({}, e);
    ENGINE_ASSERT(metaComponent, "No meta component found for component {}",
                  cPrefab)

    for (const auto& [cField, cFieldValue] : cValue.items()) {
      if (cFieldValue.is_string()) {
        metaComponent.set(entt::hashed_string{cField.data()},
                          cFieldValue.get<std::string>());
      } else if (cFieldValue.is_number_integer()) {
        metaComponent.set(entt::hashed_string{cField.data()},
                          cFieldValue.get<int>());
      } else if (cFieldValue.is_number_float()) {
        metaComponent.set(entt::hashed_string{cField.data()},
                          cFieldValue.get<float>());
      } else if (cFieldValue.is_boolean()) {
        metaComponent.set(entt::hashed_string{cField.data()},
                          cFieldValue.get<bool>());
      } else if (cFieldValue.is_array()) {
        std::vector<std::string> paths;
        paths.reserve(cFieldValue.size());
        for (const auto& value : cFieldValue) {
          ENGINE_ASSERT(value.is_string(),
                        "Unsupported type {} for component {} field {}",
                        value.type_name(), cPrefab, cField);
          paths.emplace_back(value.get<std::string>());
        }
        metaComponent.set(entt::hashed_string{cField.data()}, std::move(paths));
      } else if (cFieldValue.is_object()) {
        if (cFieldValue.contains("x") and cFieldValue.contains("y") and
            cFieldValue.contains("z") and cFieldValue.contains("w")) {
          if (cField == "rotation") {
            metaComponent.set(entt::hashed_string{cField.data()},
                              quatFromJson(cFieldValue));
          } else {
            metaComponent.set(entt::hashed_string{cField.data()},
                              vec4FromJson(cFieldValue));
          }
        } else if (cFieldValue.contains("x") and cFieldValue.contains("y") and
                   cFieldValue.contains("z")) {
          if (cField == "rotation") {
            metaComponent.set(
              entt::hashed_string{cField.data()},
              glm::quat(glm::radians(vec3FromJson(cFieldValue))));
          } else {
            metaComponent.set(entt::hashed_string{cField.data()},
                              vec3FromJson(cFieldValue));
          }
        } else if (cFieldValue.contains("x") and cFieldValue.contains("y")) {
          metaComponent.set(entt::hashed_string{cField.data()},
                            vec2FromJson(cFieldValue));
        } else if (cFieldValue.contains("r") and cFieldValue.contains("g") and
                   cFieldValue.contains("b") and cFieldValue.contains("a")) {
          metaComponent.set(entt::hashed_string{cField.data()},
                            vec4FromJson(cFieldValue, "color"));
        } else if (cFieldValue.contains("r") and cFieldValue.contains("g") and
                   cFieldValue.contains("b")) {
          metaComponent.set(entt::hashed_string{cField.data()},
                            vec3FromJson(cFieldValue, "color"));
        } else if (cField == "json") { // TODO unused
          ENGINE_ASSERT(false, "json field is not supported for component {}",
                        cPrefab);
          metaComponent.set(entt::hashed_string{cField.data()},
                            std::move(cFieldValue));
        } else {
          ENGINE_ASSERT(false, "Unsupported type {} for component {} field {}",
                        cFieldValue.type_name(), cPrefab, cField)
        }
      } else {
        ENGINE_ASSERT(false, "Unsupported type {} for component {} field {}",
                      cFieldValue.type_name(), cPrefab, cField)
      }
    }
  } else {
    ENGINE_ASSERT(false, "Unsupported type {} for component {}",
                  cValue.type_name(), cPrefab)
  }
  entt::meta_func triggerEventFunc = cType.func("onComponentAdded"_hs);
  if (triggerEventFunc) {
    triggerEventFunc.invoke({}, e, metaComponent);
  }
}

void EntityFactory::createPrototypes(
  std::string_view prefab_name,
  const std::vector<std::string>& prototypeIDs, entt::registry& registry,
  const std::unique_ptr<assets::AssetsManager>& assets_manager) {
  const auto& prefab = assets_manager->get<assets::Prefab>(prefab_name);

  auto& prefabPrototypes = m_prefabs[prefab_name.data()];
  for (std::string_view prototypeID : prototypeIDs) {
    ENGINE_ASSERT(not prefabPrototypes.contains(prototypeID.data()),
                  "Prototype {} for prefab {} already exists", prototypeID,
                  prefab_name);
    entt::entity e = registry.create();

    for (std::string_view cTag : prefab->getCTags(prototypeID)) {
      processCTag(e, cTag);
    }
    for (const auto& [cPrefab, cValue] : prefab->getComponents(prototypeID)) {
      processComponent(e, cPrefab, cValue);
    }

    prefabPrototypes.insert({prototypeID.data(), e});
  }
  m_dirty = true;
}

void EntityFactory::updatePrototypes(
  std::string_view prefab_name,
  const std::vector<std::string>& prototypeIDs, entt::registry& registry,
  const std::unique_ptr<assets::AssetsManager>& assetsManager) {
  destroyPrototypes(prefab_name, prototypeIDs, registry);
  createPrototypes(prefab_name, prototypeIDs, registry, assetsManager);
}

void EntityFactory::destroyPrototypes(
  std::string_view prefab_name,
  const std::vector<std::string>& prototypeIDs,
  entt::registry& registry) {
  for (std::string_view prototypeID : prototypeIDs) {
    ENGINE_ASSERT(m_prefabs.at(prefab_name.data()).contains(prototypeID.data()),
                  "Unknown prototype {} for prefab {}", prototypeID,
                  prefab_name);
    registry.emplace<CDeleted>(
      m_prefabs.at(prefab_name.data()).at(prototypeID.data()));
    m_prefabs.at(prefab_name.data()).erase(prototypeID.data());
  }
  m_dirty = true;
}

EntityFactory::Prototypes EntityFactory::getPrototypes(
  std::string_view prefab_name,
  const std::vector<std::string>& prototypeIDs) {
  ENGINE_ASSERT(m_prefabs.contains(prefab_name.data()), "Unknown prefab {}",
                prefab_name);
  Prototypes prototypes;
  for (std::string_view prototypeID : prototypeIDs) {
    ENGINE_ASSERT(m_prefabs.at(prefab_name.data()).contains(prototypeID.data()),
                  "Unknown prototype {} for prefab {}", prototypeID,
                  prefab_name);
    prototypes.insert(
      {prototypeID.data(),
       m_prefabs.at(prefab_name.data()).at(prototypeID.data())});
  }
  return prototypes;
}

bool EntityFactory::containsPrototypes(
  std::string_view prefab_name,
  const std::vector<std::string>& prototypeIDs) const {
  ENGINE_ASSERT(m_prefabs.contains(prefab_name.data()), "Unknown prefab {}",
                prefab_name);
  for (std::string_view prototypeID : prototypeIDs) {
    if (not m_prefabs.at(prefab_name.data()).contains(prototypeID.data())) {
      return false;
    }
  }
  return true;
}

const std::map<std::string, std::string, NumericComparator>&
EntityFactory::getPrototypesCountByPrefab() {
  if (not m_dirty) {
    return m_prototypesCountByPrefab;
  }

  m_prototypesCountByPrefab.clear();
  for (const auto& [prefab_name, prototypes] : m_prefabs) {
    m_prototypesCountByPrefab["Prefab " + prefab_name] =
      std::to_string(prototypes.size());
  }
  m_dirty = false;

  return m_prototypesCountByPrefab;
}

const std::map<std::string, EntityFactory::Prototypes, NumericComparator>&
EntityFactory::getAllPrototypes() {
  return m_prefabs;
}

void EntityFactory::clearPrototypes() {
  m_prefabs.clear();
  m_prototypesCountByPrefab.clear();
  m_dirty = false;
}

}