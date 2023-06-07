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
#ifdef DEBUG
    print();
#endif
}

Prefab::~Prefab() {
}

void Prefab::read(const json& data) {
    for (auto& c : data.at("ctags")) {
        m_ctags.emplace_back(c);
    }

    for (auto& [key, value] : data.at("components").items()) {
        m_components[key] = value;
    }
}

void Prefab::print() const {
    CORE_INFO("\tLoaded prefab: {0} with {1} components, {2} ctags, and {3} inherit", m_name, m_components.size(), m_ctags.size(), m_inherit.size());
    for (const auto& i : m_inherit) {
        CORE_INFO("\t\tInherit: {0}", i);
    }
    for (const auto& c : std::ranges::reverse_view(m_ctags)) {
        CORE_INFO("\t\tComponent tag: {0}", c);
    }
    for (const auto& [key, value] : m_components) {
        CORE_INFO("\t\tComponent: {0}", key);
    }
}

}