#include "potatoengine/scene/prefab.h"

#include "potatoengine/utils/timer.h"

namespace potatoengine {
Prefab::Prefab(const std::filesystem::path& fp, const std::string& target) : m_filepath(fp.string()), m_name(target) {
    std::ifstream f(fp);
    if (not f.is_open()) [[unlikely]] {
        f.close();
        throw std::runtime_error("Failed to load prefab: " + m_filepath);
    }
    json data = json::parse(f);
    f.close();

    auto ptarget = data.at(target);
    read(ptarget);
    ptarget.at("inherit").get_to(m_inherit);
    for (auto& i : m_inherit) {
        read(data.at(i));
    }
}

Prefab::~Prefab() {
}

void Prefab::print() const {
    CORE_INFO("\tLoaded prefab: {0} with {1} components, {2} ctags, and {3} inherit", m_name, m_components.size(), m_ctags.size(), m_inherit.size());
    for (const auto& i : m_inherit) {
        CORE_INFO("\t\tInherit: {0}", i);
    }
    for (const auto& c : std::ranges::reverse_view(m_ctags)) {
        CORE_INFO("\t\tCTag: {0}", c);
    }
    for (const auto& [key, value] : m_components) {
        if (std::holds_alternative<ftypes>(value)) {
            const auto& fvalue = std::get<ftypes>(value);
            if (std::holds_alternative<std::string>(fvalue)) {
                CORE_INFO("\t\t{0}: {1} (string)", key, std::get<std::string>(fvalue));
            } else if (std::holds_alternative<int>(fvalue)) {
                CORE_INFO("\t\t{0}: {1} (int)", key, std::get<int>(fvalue));
            } else if (std::holds_alternative<float>(fvalue)) {
                CORE_INFO("\t\t{0}: {1} (float)", key, std::get<float>(fvalue));
            } else if (std::holds_alternative<bool>(fvalue)) {
                CORE_INFO("\t\t{0}: {1} (bool)", key, std::get<bool>(fvalue));
            }
        } else if (std::holds_alternative<std::map<std::string, ftypes>>(value)) {
            CORE_INFO("\t\tComponent: {0}", key);
            for (const auto& [k, v] : std::get<std::map<std::string, ftypes>>(value)) {
                if (std::holds_alternative<std::string>(v)) {
                    CORE_INFO("\t\t\t{0}: {1} (string)", k, std::get<std::string>(v));
                } else if (std::holds_alternative<int>(v)) {
                    CORE_INFO("\t\t\t{0}: {1} (int)", k, std::get<int>(v));
                } else if (std::holds_alternative<float>(v)) {
                    CORE_INFO("\t\t\t{0}: {1} (float)", k, std::get<float>(v));
                } else if (std::holds_alternative<bool>(v)) {
                    CORE_INFO("\t\t\t{0}: {1} (bool)", k, std::get<bool>(v));
                }
            }
        }
    }
}

void Prefab::read(const json& data) {
    for (auto& c : data.at("ctags")) {
        m_ctags.emplace_back(c);
    }

    for (auto& [key, value] : data.at("components").items()) {
        if (value.is_string()) {
            m_components[key] = value.get<std::string>();
        } else if (value.is_object()) {
            std::map<std::string, ftypes> innerMap;
            for (auto& [k, v] : value.items()) {
                if (v.is_string()) {
                    innerMap[k] = v.get<std::string>();
                } else if (v.is_number_integer()) {
                    innerMap[k] = v.get<int>();
                } else if (v.is_number_float()) {
                    innerMap[k] = v.get<float>();
                } else if (v.is_boolean()) {
                    innerMap[k] = v.get<bool>();
                } else {
                    CORE_WARN("\tUnknown component property {0} and type {1}", k, v.type_name());
                }
            }
            m_components[key] = innerMap;
        } else {
            CORE_WARN("\tUnknown component property {0} and type {1}", key, value.type_name());
        }
    }
}
}