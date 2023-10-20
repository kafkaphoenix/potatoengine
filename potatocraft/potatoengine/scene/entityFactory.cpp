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

glm::quat quatFromJson(const json& data) {
    glm::quat quat{};
    quat.x = data.at("x").get<float>();
    quat.y = data.at("y").get<float>();
    quat.z = data.at("z").get<float>();
    quat.w = data.at("w").get<float>();
    return quat;
}

EntityFactory::EntityFactory(std::weak_ptr<AssetsManager> am) : m_assetsManager(am) {}

void processCTag(const std::shared_ptr<Prefab>& prefab, Entity e, std::string_view target) {
    using namespace entt::literals;

    for (std::string_view cTag : prefab->getCTags(target)) {
        entt::meta_type cType = entt::resolve(entt::hashed_string{cTag.data()});
        if (not cType) {
            throw std::runtime_error("No component type found for component tag " + std::string(cTag));
        }

        entt::meta_func assignFunc = cType.func("assign"_hs);
        if (not assignFunc) {
            throw std::runtime_error("No assign function found for component tag " + std::string(cTag));
        }
        
        entt::meta_any metaComponent = assignFunc.invoke({}, e);
        entt::meta_func triggerEventFunc = cType.func("onComponentAdded"_hs);
        if (triggerEventFunc) {
            triggerEventFunc.invoke({}, e, metaComponent);
        }
    }
}

void processComponent(Entity& e, const std::string& cPrefab, const json& cValue) {
    using namespace entt::literals;

    entt::meta_type cType = entt::resolve(entt::hashed_string{cPrefab.data()});
    if (not cType) {
        throw std::runtime_error("No component type found for component " + cPrefab);
    }

    entt::meta_func assignFunc = cType.func("assign"_hs);
    if (not assignFunc) {
        throw std::runtime_error("No assign function found for component " + cPrefab);
    }

    entt::meta_any metaComponent;
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
                paths.reserve(cFieldValue.size());
                for (const auto& value : cFieldValue) {
                    if (not value.is_string()) {
                        throw std::runtime_error("Unsupported type " + std::string{value.type_name()} + " for component " + cPrefab + " field " + cField);
                    }
                    paths.emplace_back(value.get<std::string>());
                }
                metaComponent.set(entt::hashed_string{cField.data()}, std::move(paths));
            } else if (cFieldValue.is_object()) {
                if (cFieldValue.contains("x") and cFieldValue.contains("y") and cFieldValue.contains("z") and cFieldValue.contains("w")) {
                    if (cField == "rotation") {
                        metaComponent.set(entt::hashed_string{cField.data()}, quatFromJson(cFieldValue));
                    } else {
                        metaComponent.set(entt::hashed_string{cField.data()}, vec4FromJson(cFieldValue));
                    }
                } else if (cFieldValue.contains("x") and cFieldValue.contains("y") and cFieldValue.contains("z")) {
                    if (cField == "rotation") {
                        metaComponent.set(entt::hashed_string{cField.data()}, glm::quat(glm::radians(vec3FromJson(cFieldValue))));
                    } else {
                        metaComponent.set(entt::hashed_string{cField.data()}, vec3FromJson(cFieldValue));
                    }
                } else if (cFieldValue.contains("x") and cFieldValue.contains("y")) {
                    metaComponent.set(entt::hashed_string{cField.data()}, vec2FromJson(cFieldValue));
                } else if (cFieldValue.contains("r") and cFieldValue.contains("g") and cFieldValue.contains("b") and cFieldValue.contains("a")) {
                    metaComponent.set(entt::hashed_string{cField.data()}, vec4FromJson(cFieldValue, "color"));
                } else if (cFieldValue.contains("r") and cFieldValue.contains("g") and cFieldValue.contains("b")) {
                    metaComponent.set(entt::hashed_string{cField.data()}, vec3FromJson(cFieldValue, "color"));
                } else if (cField == "json") { // TODO unused
                    metaComponent.set(entt::hashed_string{cField.data()}, std::move(cFieldValue));
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

void EntityFactory::create(std::string_view id, Entity e) {
    if (m_prototypes.contains(id.data())) {
        throw std::runtime_error("Prototype already exists for prefab " + std::string(id));
    }

    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }

    const auto& prefab = manager->get<Prefab>(id);
    auto& sceneManager = e.getSceneManager();
    e.add<CDeleted>();

    for (std::string_view target : prefab->getTargets()) {
        e = {sceneManager.getRegistry().create(), &sceneManager};

        processCTag(prefab, e, target);

        for (const auto& [cPrefab, cValue] : prefab->getComponents(target)) {
            processComponent(e, cPrefab, cValue);
        }
        
        m_prototypes.insert({target.data(), e});
    }
}

void EntityFactory::destroy(std::string_view id, entt::registry& r) {
    if (not m_prototypes.contains(id.data())) {
        throw std::runtime_error("No prototype found for prefab " + std::string(id));
    }
    r.emplace<CDeleted>(m_prototypes.at(id.data()));
    m_prototypes.erase(id.data());
}

void EntityFactory::update(std::string_view id, Entity e, entt::registry& r) {
    destroy(id, r);
    create(id, e);
}

entt::entity EntityFactory::get(std::string_view id) {
    if (not m_prototypes.contains(id.data())) {
        throw std::runtime_error("No prototype found for prefab " + std::string(id));
    }
    return m_prototypes.at(id.data());
}

void EntityFactory::clear() {
    m_prototypes.clear();
}
}