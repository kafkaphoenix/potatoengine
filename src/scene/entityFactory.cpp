#include "scene/entityFactory.h"

#include <entt/core/hashed_string.hpp>
#include <entt/meta/meta.hpp>
#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <nlohmann/json.hpp>

#include "core/application.h"
#include "pch.h"
#include "scene/components/utils/cDeleted.h"

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

void processComponent(entt::entity e, const std::string& cPrefab,
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

void EntityFactory::createPrototypes(std::string_view prefabID,
                                     entt::registry& registry) {
  ENGINE_ASSERT(not m_prefabs.contains(prefabID.data()),
                "Prototypes for prefab {} already exist", prefabID);

  const auto& assetsManager = Application::Get().getAssetsManager();
  const auto& prefab = assetsManager->get<Prefab>(prefabID);

  Prototypes prototypes;
  for (const auto& [prototypeID, prototypeData] : prefab->getPrototypes()) {
    entt::entity e = registry.create();

    for (std::string_view cTag : prototypeData.ctags) {
      processCTag(e, cTag);
    }

    for (const auto& [cPrefab, cValue] : prototypeData.components) {
      processComponent(e, cPrefab, cValue);
    }

    prototypes.insert({prototypeID, e});
  }
  m_prefabs.insert({prefabID.data(), std::move(prototypes)});
  m_dirty = true;
}

void EntityFactory::updatePrototypes(std::string_view prefabID,
                                     entt::registry& registry) {
  ENGINE_ASSERT(m_prefabs.contains(prefabID.data()), "Unknown prefab {}",
                prefabID);
  destroyPrototypes(prefabID, registry);
  createPrototypes(prefabID, registry);
  m_dirty = true;
}

void EntityFactory::destroyPrototypes(std::string_view prefabID,
                                      entt::registry& registry) {
  ENGINE_ASSERT(m_prefabs.contains(prefabID.data()), "Unknown prefab {}",
                prefabID);
  for (auto& [prototypeID, prototype] : m_prefabs.at(prefabID.data())) {
    registry.emplace<CDeleted>(prototype);
  }
  m_prefabs.erase(prefabID.data());
  m_dirty = true;
}

const EntityFactory::Prototypes&
EntityFactory::getPrototypes(std::string_view prefabID) {
  ENGINE_ASSERT(m_prefabs.contains(prefabID.data()), "Unknown prefab {}",
                prefabID);
  return m_prefabs.at(prefabID.data());
}

const std::map<std::string, EntityFactory::Prototypes, NumericComparator>&
EntityFactory::getAllPrototypes() {
  return m_prefabs;
}

bool EntityFactory::containsPrototypes(std::string_view prefabID) const {
  return m_prefabs.contains(prefabID.data());
}

void EntityFactory::createPrototype(std::string_view prefabID,
                                    std::string_view prototypeID,
                                    entt::registry& registry) {
  ENGINE_ASSERT(m_prefabs.contains(prefabID.data()), "Unknown prefab {}",
                prefabID);
  ENGINE_ASSERT(not m_prefabs.at(prefabID.data()).contains(prototypeID.data()),
                "Prototype {} for prefab {} already exists", prototypeID,
                prefabID);

  const auto& assetsManager = Application::Get().getAssetsManager();
  const auto& prefab = assetsManager->get<Prefab>(prefabID);
  entt::entity e = registry.create();

  for (std::string_view cTag : prefab->getCTags(prototypeID)) {
    processCTag(e, cTag);
  }

  for (const auto& [cPrefab, cValue] : prefab->getComponents(prototypeID)) {
    processComponent(e, cPrefab, cValue);
  }

  m_prefabs.at(prefabID.data()).insert({prototypeID.data(), e});
  m_dirty = true;
}

void EntityFactory::updatePrototype(std::string_view prefabID,
                                    std::string_view prototypeID,
                                    entt::registry& registry) {
  ENGINE_ASSERT(m_prefabs.contains(prefabID.data()), "Unknown prefab {}",
                prefabID);
  ENGINE_ASSERT(m_prefabs.at(prefabID.data()).contains(prototypeID.data()),
                "Unknown prototype {} for prefab {}", prototypeID, prefabID);
  destroyPrototype(prefabID, prototypeID, registry);
  createPrototype(prefabID, prototypeID, registry);
  m_dirty = true;
}

void EntityFactory::destroyPrototype(std::string_view prefabID,
                                     std::string_view prototypeID,
                                     entt::registry& registry) {
  ENGINE_ASSERT(m_prefabs.contains(prefabID.data()), "Unknown prefab {}",
                prefabID);
  ENGINE_ASSERT(m_prefabs.at(prefabID.data()).contains(prototypeID.data()),
                "Unknown prototype {} for prefab {}", prototypeID, prefabID);
  registry.emplace<CDeleted>(
    m_prefabs.at(prefabID.data()).at(prototypeID.data()));
  m_prefabs.at(prefabID.data()).erase(prototypeID.data());
  m_dirty = true;
}

entt::entity EntityFactory::getPrototype(std::string_view prefabID,
                                         std::string_view prototypeID) {
  ENGINE_ASSERT(m_prefabs.contains(prefabID.data()), "Unknown prefab {}",
                prefabID);
  ENGINE_ASSERT(m_prefabs.at(prefabID.data()).contains(prototypeID.data()),
                "Unknown prototype {} for prefab {}", prototypeID, prefabID);
  return m_prefabs.at(prefabID.data()).at(prototypeID.data());
}

bool EntityFactory::containsPrototype(std::string_view prefabID,
                                      std::string_view prototypeID) const {
  ENGINE_ASSERT(m_prefabs.contains(prefabID.data()), "Unknown prefab {}",
                prefabID);
  return m_prefabs.at(prefabID.data()).contains(prototypeID.data());
}

const std::map<std::string, std::string, NumericComparator>&
EntityFactory::getPrototypesCountByPrefab() {
  if (not m_dirty) {
    return m_prototypesCountByPrefab;
  }

  m_prototypesCountByPrefab.clear();
  for (const auto& [prefabID, prototypes] : m_prefabs) {
    m_prototypesCountByPrefab["Prefab " + prefabID] =
      std::to_string(prototypes.size());
  }
  m_dirty = false;

  return m_prototypesCountByPrefab;
}

void EntityFactory::clear() {
  m_prefabs.clear();
  m_prototypesCountByPrefab.clear();
  m_dirty = false;
}

}