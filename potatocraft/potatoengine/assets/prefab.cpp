#include "potatoengine/assets/prefab.h"

#include "potatoengine/utils/timer.h"

namespace potatoengine {
Prefab::Prefab(std::filesystem::path&& fp, std::unordered_set<std::string>&& targets) : m_filepath(std::move(fp.string())), m_targets(std::move(targets)) {
    std::ifstream f(fp);
    if (not f.is_open()) [[unlikely]] {
        f.close();
        throw std::runtime_error("Failed to load prefab: " + m_filepath);
    }
    json data = json::parse(f);
    f.close();

    for (const auto& [name, prefabData] : data.items()) {
        if (not m_targets.contains(name)) {
            continue;
        }
        std::unordered_set<std::string> inherits;
        std::unordered_set<std::string> ctags;
        std::unordered_map<std::string, json> components;

        if (prefabData.contains("inherit")) {
            prefabData.at("inherit").get_to(inherits);
            for (std::string_view father : inherits) {
                read(data.at(father), inherits, ctags, components);
            }
        }
        read(prefabData, inherits, ctags, components);  // child overrides parent if common definition exists

        m_prefabs[name] = {.inherits = std::move(inherits), .ctags = std::move(ctags), .components = std::move(components)};
    }

#ifdef DEBUG
    print();
#endif
}

void Prefab::read(const json& data, std::unordered_set<std::string>& inherits, std::unordered_set<std::string>& ctags, std::unordered_map<std::string, json>& components) {
    if (data.contains("ctags")) {
        for (const json& c : data.at("ctags")) {
            ctags.emplace(c);
        }
    }

    if (data.contains("components")) {
        for (const auto& [cKey, cValue] : data.at("components").items()) {
            if (inherits.size() > 0 and ctags.contains(cKey)) {
                ctags.erase(cKey);
            }
            if (components.contains(cKey)) {
                for (const auto& [cFieldKey, cFieldValue] : cValue.items()) {
                    if (not cFieldKey.empty()) {
                        components[cKey][cFieldKey] = cFieldValue;
                    } else {
                        components[cKey] = cFieldValue;
                    }
                }
            } else {
                components[cKey] = cValue;
            }
        }
    }
}

void Prefab::print() const {
    for (const auto& [name, prefabData] : m_prefabs) {
        CORE_INFO("\tLoaded prefab: {0} with {1} inherits, {2} ctags and {3} components", name, prefabData.inherits.size(), prefabData.ctags.size(), prefabData.components.size());
        for (std::string_view father : prefabData.inherits) {
            CORE_INFO("\t\tInherit: {0}", father);
        }
        for (std::string_view ctag : prefabData.ctags) {
            CORE_INFO("\t\tComponent tag: {0}", ctag);
        }
        for (const auto& [cKey, _] : prefabData.components) {
            CORE_INFO("\t\tComponent: {0}", cKey);
        }
    }
}
}