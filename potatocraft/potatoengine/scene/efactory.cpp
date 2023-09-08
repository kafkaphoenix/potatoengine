#include "potatoengine/scene/efactory.h"

#include "potatoengine/pch.h"
#include <entt/core/hashed_string.hpp>
#include <entt/meta/meta.hpp>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "potatoengine/scene/components.h"
#include "potatoengine/scene/entity.h"
using json = nlohmann::json;

namespace potatoengine {

// cppcheck-suppress unusedFunction
void to_json(json& j, const glm::vec3& v) {
    j = json{{"x", v.x}, {"y", v.y}, {"z", v.z}};
}

void from_json(const json& j, glm::vec3& v) {
    v.x = j.at("x").get<float>();
    v.y = j.at("y").get<float>();
    v.z = j.at("z").get<float>();
}

Efactory::Efactory(std::weak_ptr<AssetsManager> am) : m_assetsManager(am) {}

void Efactory::create(assets::PrefabID id, Entity e) {
    using namespace entt::literals;

    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }
    const auto& pfb = manager->get<Prefab>(id);
    for (auto&& t : pfb->getCTags()) {
        auto ctype = entt::resolve(entt::hashed_string{t.c_str()});
        if (not ctype) {
            CORE_ERROR("No component type found for component tag {0}", t);
            continue;
        }
        auto assignFunc = ctype.func("assign"_hs);
        if (assignFunc) {
            assignFunc.invoke({}, e);
        } else {
            CORE_ERROR("No assign function found for component tag {0}", t);
        }
    }

    for (auto&& c : pfb->getComponents()) {
        auto ctype = entt::resolve(entt::hashed_string{c.first.c_str()});
        if (not ctype) {
            CORE_ERROR("No component type found for component {0}", c.first);
            continue;
        }
        auto assignValuesFunc = ctype.func("assignValues"_hs);
        if (assignValuesFunc) {
            auto v = c.second;
            if (v.is_string()) {
                auto str = v.get<std::string>();
                if (c.first == "body") {
                    auto assignFunc = ctype.func("assign"_hs);
                    if (not assignFunc) {
                        CORE_ERROR("No assign function found for component {0}", c.first);
                        continue;
                    }
                    auto ec = assignFunc.invoke({}, e);
                    loadModel(str, ec);
                } else {
                    assignValuesFunc.invoke({}, e, str);
                }
            } else if (v.is_number_integer()) {
                assignValuesFunc.invoke({}, e, v.get<int>());
            } else if (v.is_number_float()) {
                assignValuesFunc.invoke({}, e, v.get<float>());
            } else if (v.is_boolean()) {
                assignValuesFunc.invoke({}, e, v.get<bool>());
            } else if (v.is_object() and v.contains("x") and v.contains("y") and v.contains("z")) {
                glm::vec3 vec{};
                from_json(v, vec);
                assignValuesFunc.invoke({}, e, vec);
            } else if (v.is_object()) {
                auto assignFunc = ctype.func("assign"_hs);
                if (not assignFunc) {
                    CORE_ERROR("No assign function found for component {0}", c.first);
                    continue;
                }
                auto ec = assignFunc.invoke({}, e);
                for (auto&& [k, _v] : v.items()) {
                    if (_v.is_string()) {
                        if (c.first == "textureAtlas" and k == "filepath") {
                            loadAtlas(_v.get<std::string>(), ec);
                        } else {
                            ec.set(entt::hashed_string{k.c_str()}, _v.get<std::string>());
                        }
                    } else if (_v.is_number_integer()) {
                        ec.set(entt::hashed_string{k.c_str()}, _v.get<int>());
                    } else if (_v.is_number_float()) {
                        ec.set(entt::hashed_string{k.c_str()}, _v.get<float>());
                    } else if (_v.is_boolean()) {
                        ec.set(entt::hashed_string{k.c_str()}, _v.get<bool>());
                    } else if (_v.is_object() and _v.contains("x") and _v.contains("y") and _v.contains("z")) {
                        glm::vec3 vec{};
                        from_json(_v, vec);
                        ec.set(entt::hashed_string{k.c_str()}, vec);
                    } else {
                        CORE_ERROR("Unsupported type {0} for component {1} field {2}", _v.type_name(), c.first, k);
                    }
                }
            } else {
                CORE_ERROR("Unsupported type {0} for component {1}", v.type_name(), c.first);
            }
        } else {
            CORE_ERROR("No assign values function found for component {0}", c.first);
        }
    }
    m_protos[id] = e;
}

void Efactory::destroy(assets::PrefabID id, entt::registry& r) {
    if (not m_protos.contains(id)) {
        throw std::runtime_error("No prototype found for prefab " + assets::to_string(id));
    }
    r.emplace<Deleted>(m_protos.at(id));
    m_protos.erase(id);
}

void Efactory::update(assets::PrefabID id, Entity e, entt::registry& r) {
    destroy(id, r);
    create(id, e);
}

entt::entity Efactory::get(assets::PrefabID id) {
    if (not m_protos.contains(id)) {
        throw std::runtime_error("No prototype found for prefab " + assets::to_string(id));
    }
    return m_protos.at(id);
}

void Efactory::loadModel(const std::string& filepath, entt::meta_any& ec) {
    using namespace entt::literals;

    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }
    if (not manager->exists<Model>(filepath)) {
        manager->load<Model>(filepath, filepath);
    }
    const auto& model = manager->get<Model>(filepath);
    ec.set(entt::hashed_string{"filepath"_hs}, filepath);
    ec.set(entt::hashed_string{"meshes"_hs}, model->getMeshes());
    ec.set(entt::hashed_string{"materials"_hs}, model->getMaterials());
}

void Efactory::loadAtlas(const std::string& filepath, entt::meta_any& ec) {
    using namespace entt::literals;

    const auto& manager = m_assetsManager.lock();
    if (not manager) {
        throw std::runtime_error("Assets manager is null!");
    }
    if (not manager->exists<Texture>(filepath)) {
        manager->load<Texture>(filepath, filepath);
    }
    const auto& texture = manager->get<Texture>(filepath);
    ec.set(entt::hashed_string{"texture"_hs}, texture);
}
}