#include "potatoengine/assets/prefab.h"

#include "potatoengine/utils/timer.h"

namespace potatoengine {
Prefab::Prefab(const std::filesystem::path& fp, const std::string& name) : m_name(name) {
    m_filepath = fp.string();

    std::ifstream f(fp);
    if (not f.is_open()) [[unlikely]] {
        f.close();
        throw std::runtime_error("Failed to load prefab: " + m_filepath);
    }
    json data = json::parse(f);
    f.close();

    const json& prefabData = data.at(name);
    if (prefabData.contains("inherit")) {
        prefabData.at("inherit").get_to(m_inherits);
        for (const std::string& father : m_inherits) {
            read(data.at(father));
        }
    }
    read(prefabData);  // child overrides parent if common definition exists
#ifdef DEBUG
    print();
#endif
}

void Prefab::read(const json& data) {
    if (data.contains("ctags")) {
        for (const json& c : data.at("ctags")) {
                m_ctags.insert(c);
        }
    }

    if (data.contains("components")) {
        for (const auto& [cKey, cValue] : data.at("components").items()) {
            if (m_inherits.size() > 0 and m_ctags.contains(cKey)) {
                m_ctags.erase(cKey);
            }
            if (m_components.contains(cKey)) {
                for (const auto& [cFieldKey, cFieldValue] : cValue.items()) {
                    if (not cFieldKey.empty()) {
                        m_components[cKey][cFieldKey] = cFieldValue;
                    } else {
                        m_components[cKey] = cFieldValue;
                    }
                }
            } else {
                m_components[cKey] = cValue;
            }
        }
    }

}

void Prefab::print() const {
    CORE_INFO("\tLoaded prefab: {0} with {1} components, {2} ctags, and {3} inherit", m_name, m_components.size(), m_ctags.size(), m_inherits.size());
    for (std::string_view father : m_inherits) {
        CORE_INFO("\t\tInherit: {0}", father);
    }
    for (std::string_view ctag : std::ranges::reverse_view(m_ctags)) {
        CORE_INFO("\t\tComponent tag: {0}", ctag);
    }
    for (const auto& [cKey, _] : m_components) {
        CORE_INFO("\t\tComponent: {0}", cKey);
    }
}

}