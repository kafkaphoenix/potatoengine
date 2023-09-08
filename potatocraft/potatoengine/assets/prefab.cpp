#include "potatoengine/assets/prefab.h"

#include "potatoengine/utils/timer.h"

namespace potatoengine {
Prefab::Prefab(const std::filesystem::path& fp, const std::string& name): m_name(name) {
    if (not std::filesystem::exists(fp)) [[unlikely]] {
        throw std::runtime_error("Prefab file does not exist: " + m_filepath);
    }
    m_filepath = fp.string();

    std::ifstream f(fp);
    if (not f.is_open()) [[unlikely]] {
        f.close();
        throw std::runtime_error("Failed to load prefab: " + m_filepath);
    }
    json data = json::parse(f);
    f.close();

    auto pfb = data.at(name);
    pfb.at("inherit").get_to(m_inherits);
    for (const auto& i : m_inherits) {
        read(data.at(i));
    }
    read(pfb);
#ifdef DEBUG
    print();
#endif
}

void Prefab::read(const json& j) {
    for (auto& c : j.at("ctags")) {
        m_ctags.emplace_back(c);
    }

    for (auto& [k, v] : j.at("components").items()) {
        if (m_components.contains(k)) {
            for (auto& [kk, vv] : v.items()) {
                m_components[k][kk] = vv;
            }
        } else {
            m_components[k] = v;
        }
    }
}

void Prefab::print() const {
    CORE_INFO("\tLoaded prefab: {0} with {1} components, {2} ctags, and {3} inherit", m_name, m_components.size(), m_ctags.size(), m_inherits.size());
    for (const auto& i : m_inherits) {
        CORE_INFO("\t\tInherit: {0}", i);
    }
    for (const auto& c : std::ranges::reverse_view(m_ctags)) {
        CORE_INFO("\t\tComponent tag: {0}", c);
    }
    for (const auto& [k, v] : m_components) {
        CORE_INFO("\t\tComponent: {0}", k);
    }
}

}