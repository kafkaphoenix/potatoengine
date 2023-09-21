#include "potatoengine/scene/entityFactory.h"

#include <entt/core/hashed_string.hpp>
#include <entt/meta/meta.hpp>
#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/scene/components/utils/cDeleted.h"

#include "potatoengine/scene/entity.h"
using json = nlohmann::json;

namespace potatoengine {

// cppcheck-suppress unusedFunction
json toJson(const glm::vec3& cFieldValue) {
    return json{{"x", cFieldValue.x}, {"y", cFieldValue.y}, {"z", cFieldValue.z}};
}

glm::vec2 vec2FromJson(const json& data) {
    glm::vec2 vec{};
    vec.x = data.at("x").get<float>();
    vec.y = data.at("y").get<float>();
    return vec;
}

glm::vec3 vec3FromJson(const json& data, std::string_view type = "") {
    glm::vec3 vec{};
    vec.x = (type == "color") ? data.at("r").get<float>() : data.at("x").get<float>();
    vec.y = (type == "color") ? data.at("g").get<float>() : data.at("y").get<float>();
    vec.z = (type == "color") ? data.at("b").get<float>() : data.at("z").get<float>();
    return vec;
}

glm::vec4 vec4FromJson(const json& data, std::string_view type = "") {
    glm::vec4 vec{};
    vec.x = (type == "color") ? data.at("r").get<float>() : data.at("x").get<float>();
    vec.y = (type == "color") ? data.at("g").get<float>() : data.at("y").get<float>();
    vec.z = (type == "color") ? data.at("b").get<float>() : data.at("z").get<float>();
    vec.w = (type == "color") ? data.at("a").get<float>() : data.at("w").get<float>();
    return vec;
}

EntityFactory::EntityFactory(std::weak_ptr<AssetsManager> am) : m_assetsManager(am) {}

void EntityFactory::create(assets::PrefabID id, Entity e) {
    using namespace entt::literals;

    if (m_prototypes.contains(id)) {
        throw std::runtime_error("Prototype already exists for prefab " + assets::to_string(id));
    }

    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }

    const auto& prefab = manager->get<Prefab>(id);
    entt::meta_type cType;
    entt::meta_any metaComponent;
    for (std::string_view cTag : prefab->getCTags()) {
        cType = entt::resolve(entt::hashed_string{cTag.data()});
        if (not cType) {
            throw std::runtime_error("No component type found for component tag " + std::string(cTag));
        }
        entt::meta_func assignFunc = cType.func("assign"_hs);
        if (not assignFunc) {
            throw std::runtime_error("No assign function found for component tag " + std::string(cTag));
        }
        metaComponent = assignFunc.invoke({}, e);
        entt::meta_func triggerEventFunc = cType.func("onComponentAdded"_hs);
        if (triggerEventFunc) {
            triggerEventFunc.invoke({}, e, metaComponent);
        }
    }

    for (const auto& [cPrefab, cValue] : prefab->getComponents()) {
        cType = entt::resolve(entt::hashed_string{cPrefab.data()});
        if (not cType) {
            throw std::runtime_error("No component type found for component " + cPrefab);
        }
        entt::meta_func assignFunc = cType.func("assign"_hs);
        if (not assignFunc) {
            throw std::runtime_error("No assign function found for component " + cPrefab);
        }
        if (cValue.is_string()) {
            metaComponent = assignFunc.invoke({}, e, cValue.get<std::string>());
        } else if (cValue.is_number_integer()) {
            metaComponent = assignFunc.invoke({}, e, cValue.get<int>());
        } else if (cValue.is_number_float()) {
            metaComponent = assignFunc.invoke({}, e, cValue.get<float>());
        } else if (cValue.is_boolean()) {
            metaComponent = assignFunc.invoke({}, e, cValue.get<bool>());
        } else if (cValue.is_object() and cValue.contains("x") and cValue.contains("y") and cValue.contains("z")) {
            metaComponent = assignFunc.invoke({}, e, vec3FromJson(cValue));
        } else if (cValue.is_object()) {
            metaComponent = assignFunc.invoke({}, e);
            if (not metaComponent) {
                throw std::runtime_error("No meta component found for component " + cPrefab);
            }
            for (const auto& [cField, cFieldValue] : cValue.items()) {
                if (cFieldValue.is_string()) {
                    metaComponent.set(entt::hashed_string{cField.data()}, cFieldValue.get<std::string>());
                } else if (cFieldValue.is_number_integer()) {
                    metaComponent.set(entt::hashed_string{cField.data()}, cFieldValue.get<int>());
                } else if (cFieldValue.is_number_float()) {
                    metaComponent.set(entt::hashed_string{cField.data()}, cFieldValue.get<float>());
                } else if (cFieldValue.is_boolean()) {
                    metaComponent.set(entt::hashed_string{cField.data()}, cFieldValue.get<bool>());
                } else if (cFieldValue.is_array()) {
                    std::vector<std::string> paths;
                    for (const auto& value : cFieldValue) {
                        if (not value.is_string()) {
                            throw std::runtime_error("Unsupported type " + std::string{value.type_name()} + " for component " + cPrefab + " field " + cField);
                        }
                        paths.emplace_back(value.get<std::string>());
                    }
                    metaComponent.set(entt::hashed_string{cField.data()}, std::move(paths));
                } else if (cFieldValue.is_object()) {
                    if (cFieldValue.contains("x") and cFieldValue.contains("y") and cFieldValue.contains("z")) {
                        metaComponent.set(entt::hashed_string{cField.data()}, vec3FromJson(cFieldValue));
                    } else if (cFieldValue.contains("x") and cFieldValue.contains("y")) {
                        metaComponent.set(entt::hashed_string{cField.data()}, vec2FromJson(cFieldValue));
                    } else if (cFieldValue.contains("r") and cFieldValue.contains("g") and cFieldValue.contains("b") and cFieldValue.contains("a")) {
                        metaComponent.set(entt::hashed_string{cField.data()}, vec4FromJson(cFieldValue, "color"));
                    } else if (cFieldValue.contains("r") and cFieldValue.contains("g") and cFieldValue.contains("b")) {
                        metaComponent.set(entt::hashed_string{cField.data()}, vec3FromJson(cFieldValue, "color"));
                    } else {
                        throw std::runtime_error("Unsupported type " + std::string{cFieldValue.type_name()} + " for component " + cPrefab + " field " + cField);
                    }
                } else {
                    throw std::runtime_error("Unsupported type " + std::string{cFieldValue.type_name()} + " for component " + cPrefab + " field " + cField);
                }
            }
        } else {
            throw std::runtime_error("Unsupported type " + std::string{cValue.type_name()} + " for component " + cPrefab);
        }
        entt::meta_func triggerEventFunc = cType.func("onComponentAdded"_hs);
        if (triggerEventFunc) {
            triggerEventFunc.invoke({}, e, metaComponent);
        }
    }
    m_prototypes[id] = e;
}

void EntityFactory::destroy(assets::PrefabID id, entt::registry& r) {
    if (not m_prototypes.contains(id)) {
        throw std::runtime_error("No prototype found for prefab " + assets::to_string(id));
    }
    r.emplace<CDeleted>(m_prototypes.at(id));
    m_prototypes.erase(id);
}

void EntityFactory::update(assets::PrefabID id, Entity e, entt::registry& r) {
    destroy(id, r);
    create(id, e);
}

entt::entity EntityFactory::get(assets::PrefabID id) {
    if (not m_prototypes.contains(id)) {
        throw std::runtime_error("No prototype found for prefab " + assets::to_string(id));
    }
    return m_prototypes.at(id);
}
}