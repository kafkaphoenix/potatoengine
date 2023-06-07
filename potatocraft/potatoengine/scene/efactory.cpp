#include "potatoengine/scene/efactory.h"

#include <entt/core/hashed_string.hpp>
#include <entt/meta/meta.hpp>

#include "potatoengine/scene/components.h"
#include "potatoengine/scene/entity.h"

namespace potatoengine {
Efactory::Efactory(const std::shared_ptr<AssetsManager>& am) : m_assetsManager(am) {}

void Efactory::create(const asset::prefab::ID p, Entity e) {
    using namespace entt::literals;

    const auto& pfb = m_assetsManager->get<Prefab>(p).get();
    for (auto&& t : pfb.getCTags()) {
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

    for (auto&& c : pfb.getComponents()) {
        auto ctype = entt::resolve(entt::hashed_string{c.first.c_str()});
        if (not ctype) {
            CORE_ERROR("No component type found for component {0}", c.first);
            continue;
        }
        auto assignValuesFunc = ctype.func("assignValues"_hs);
        if (assignValuesFunc) {
            auto v = c.second;
            if (v.is_string()) {
                assignValuesFunc.invoke({}, e, v.get<std::string>());
            } else if (v.is_number_integer()) {
                assignValuesFunc.invoke({}, e, v.get<int>());
            } else if (v.is_number_float()) {
                assignValuesFunc.invoke({}, e, v.get<float>());
            } else if (v.is_boolean()) {
                assignValuesFunc.invoke({}, e, v.get<bool>());
            } else if (v.is_object()) {
                auto assignFunc = ctype.func("assign"_hs);
                if (not assignFunc) {
                    CORE_ERROR("No assign function found for component {0}", c.first);
                    continue;
                }
                auto ec = assignFunc.invoke({}, e);
                for (auto&& [k, _v] : v.items()) {
                    if (_v.is_string()) {
                        ec.set(entt::hashed_string{k.c_str()}, _v.get<std::string>());
                    } else if (_v.is_number_integer()) {
                        ec.set(entt::hashed_string{k.c_str()}, _v.get<int>());
                    } else if (_v.is_number_float()) {
                        ec.set(entt::hashed_string{k.c_str()}, _v.get<float>());
                    } else if (_v.is_boolean()) {
                        ec.set(entt::hashed_string{k.c_str()}, _v.get<bool>());
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
    m_protos[p] = e;
}

void Efactory::destroy(const asset::prefab::ID p, entt::registry& r) {
    if (not m_protos.contains(p)) {
        throw std::runtime_error("No prototype found for prefab " + std::string(asset::to_string(p)));
    }
    r.emplace<Deleted>(m_protos.at(p));
    m_protos.erase(p);
}

void Efactory::update(const asset::prefab::ID p, Entity e, entt::registry& r) {
    destroy(p, r);
    create(p, e);
}

entt::entity Efactory::get(const asset::prefab::ID p) {
    if (not m_protos.contains(p)) {
        throw std::runtime_error("No prototype found for prefab " + std::string(asset::to_string(p)));
    }
    return m_protos.at(p);
}
}