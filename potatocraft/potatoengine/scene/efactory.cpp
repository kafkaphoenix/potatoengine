#include "potatoengine/scene/efactory.h"

#include <entt/core/hashed_string.hpp>
#include <entt/meta/meta.hpp>
#define GLM_FORCE_CTOR_INIT

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "potatoengine/pch.h"
#include "potatoengine/scene/components.h"
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

glm::vec3 vec3FromJson(const json& data, const std::string& type = "") {
    glm::vec3 vec{};
    vec.x = (type == "color") ? data.at("r").get<float>() : data.at("x").get<float>();
    vec.y = (type == "color") ? data.at("g").get<float>() : data.at("y").get<float>();
    vec.z = (type == "color") ? data.at("b").get<float>() : data.at("z").get<float>();
    return vec;
}

glm::vec4 vec4FromJson(const json& data, const std::string& type = "") {
    glm::vec4 vec{};
    vec.x = (type == "color") ? data.at("r").get<float>() : data.at("x").get<float>();
    vec.y = (type == "color") ? data.at("g").get<float>() : data.at("y").get<float>();
    vec.z = (type == "color") ? data.at("b").get<float>() : data.at("z").get<float>();
    vec.w = (type == "color") ? data.at("a").get<float>() : data.at("w").get<float>();
    return vec;
}

Efactory::Efactory(std::weak_ptr<AssetsManager> am) : m_assetsManager(am) {}

void Efactory::create(assets::PrefabID id, Entity e) {
    using namespace entt::literals;

    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }

    const auto& prefab = manager->get<Prefab>(id);
    for (const std::string& cTag : prefab->getCTags()) {
        entt::meta_type cType = entt::resolve(entt::hashed_string{cTag.data()});
        if (not cType) {
            throw std::runtime_error("No component type found for component tag " + cTag);
        }
        entt::meta_func assignFunc = cType.func("assign"_hs);
        if (assignFunc) {
            assignFunc.invoke({}, e);
        } else {
            throw std::runtime_error("No assign function found for component tag " + cTag);
        }
    }

    for (const auto& [cPrefab, cValue] : prefab->getComponents()) {
        entt::meta_type cType = entt::resolve(entt::hashed_string{cPrefab.data()});
        if (not cType) {
            throw std::runtime_error("No component type found for component " + cPrefab);
        }
        entt::meta_func assignValuesFunc = cType.func("assignValues"_hs);
        if (assignValuesFunc) {
            if (cValue.is_string()) {
                if (cPrefab == "body") {
                    entt::meta_func assignFunc = cType.func("assign"_hs);
                    if (not assignFunc) {
                        throw std::runtime_error("No assign function found for component " + cPrefab);
                    }
                    entt::meta_any metaComponent = assignFunc.invoke({}, e);
                    loadModel(cValue.get<std::string>(), metaComponent);
                } else {
                    assignValuesFunc.invoke({}, e, cValue.get<std::string>());
                }
            } else if (cValue.is_number_integer()) {
                assignValuesFunc.invoke({}, e, cValue.get<int>());
            } else if (cValue.is_number_float()) {
                assignValuesFunc.invoke({}, e, cValue.get<float>());
            } else if (cValue.is_boolean()) {
                assignValuesFunc.invoke({}, e, cValue.get<bool>());
            } else if (cValue.is_object() and cValue.contains("x") and cValue.contains("y") and cValue.contains("z")) {
                assignValuesFunc.invoke({}, e, vec3FromJson(cValue));
            } else if (cValue.is_object()) {
                entt::meta_func assignFunc = cType.func("assign"_hs);
                if (not assignFunc) {
                    throw std::runtime_error("No assign function found for component " + cPrefab);
                }
                entt::meta_any metaComponent = assignFunc.invoke({}, e);
                for (const auto& [cField, cFieldValue] : cValue.items()) {
                    if (cFieldValue.is_string()) {
                        if (cPrefab == "body" and cField == "filepath") {
                            loadModel(cFieldValue.get<std::string>(), metaComponent);
                        } else if (cPrefab == "light" and cField == "type") {
                            loadLightType(cFieldValue.get<std::string>(), metaComponent);
                        } else {
                            metaComponent.set(entt::hashed_string{cField.data()}, cFieldValue.get<std::string>());
                        }
                    } else if (cFieldValue.is_number_integer()) {
                        int _cFieldValue = cFieldValue.get<int>();
                        if (cPrefab == "time" and cField == "startingTime") {
                            metaComponent.set(entt::hashed_string{"seconds"}, _cFieldValue * 3600.f);
                        }
                        metaComponent.set(entt::hashed_string{cField.data()}, _cFieldValue);
                    } else if (cFieldValue.is_number_float()) {
                        float _cFieldValue = cFieldValue.get<float>();
                        if (cPrefab == "time" and cField == "startingTime") {
                            metaComponent.set(entt::hashed_string{"seconds"}, _cFieldValue * 3600.f);
                        }
                        metaComponent.set(entt::hashed_string{cField.data()}, _cFieldValue);
                    } else if (cFieldValue.is_boolean()) {
                        metaComponent.set(entt::hashed_string{cField.data()}, cFieldValue.get<bool>());
                    } else if (cFieldValue.is_array()) {
                        if (cPrefab == "texture" and cField == "filepaths") {
                            loadTextures(cFieldValue, metaComponent);
                        } else {
                            throw std::runtime_error("Unsupported type " + std::string{cFieldValue.type_name()} + " for component " + cPrefab + " field " + cField);
                        }
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
        } else {
            throw std::runtime_error("No assign values function found for component " + cPrefab);
        }
    }
    m_prototypes[id] = e;
}

void Efactory::destroy(assets::PrefabID id, entt::registry& r) {
    if (not m_prototypes.contains(id)) {
        throw std::runtime_error("No prototype found for prefab " + assets::to_string(id));
    }
    r.emplace<Deleted>(m_prototypes.at(id));
    m_prototypes.erase(id);
}

void Efactory::update(assets::PrefabID id, Entity e, entt::registry& r) {
    destroy(id, r);
    create(id, e);
}

entt::entity Efactory::get(assets::PrefabID id) {
    if (not m_prototypes.contains(id)) {
        throw std::runtime_error("No prototype found for prefab " + assets::to_string(id));
    }
    return m_prototypes.at(id);
}

void Efactory::loadModel(const std::string& filepath, entt::meta_any& metaComponent) {
    using namespace entt::literals;

    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }

    auto& model = manager->get<Model>(filepath);
    metaComponent.set(entt::hashed_string{"filepath"_hs}, filepath);
    metaComponent.set(entt::hashed_string{"meshes"_hs}, model->getMeshes());
    metaComponent.set(entt::hashed_string{"materials"_hs}, model->getMaterials());
}

void Efactory::loadTextures(const json& data, entt::meta_any& metaComponent) {
    using namespace entt::literals;

    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }

    std::vector<std::shared_ptr<Texture>> textures;
    for (const json& filepath : data) {
        textures.push_back(manager->get<Texture>(filepath.get<std::string>()));
    }

    metaComponent.set(entt::hashed_string{"textures"_hs}, std::move(textures));
}

void Efactory::loadLightType(const std::string& type, entt::meta_any& metaComponent) {
    using namespace entt::literals;

    if (type == "directional") {
        metaComponent.set(entt::hashed_string{"type"_hs}, Light::Type::Directional);
    } else if (type == "point") {
        metaComponent.set(entt::hashed_string{"type"_hs}, Light::Type::Point);
    } else if (type == "spot") {
        metaComponent.set(entt::hashed_string{"type"_hs}, Light::Type::Spot);
    } else if (type == "area") {
        metaComponent.set(entt::hashed_string{"type"_hs}, Light::Type::Area);
    } else {
        throw std::runtime_error("Unknown light type " + type);
    }
}

}