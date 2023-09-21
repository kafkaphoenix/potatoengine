#pragma once

#include <nlohmann/json.hpp>

#include "potatoengine/pch.h"
using json = nlohmann::json;

namespace potatoengine {
class Prefab {
   public:
    Prefab(std::filesystem::path&& fp, std::string&& name);

    std::string_view getName() const noexcept { return m_name; }
    std::string_view getFilepath() const noexcept { return m_filepath; }
    const std::vector<std::string>& getInherits() const noexcept { return m_inherits; }
    const std::unordered_map<std::string, nlohmann::json>& getComponents() const noexcept { return m_components; }
    bool hasComponent(std::string_view name) const noexcept { return m_components.contains(name.data()); }
    const std::unordered_set<std::string>& getCTags() const noexcept { return m_ctags; }
    bool hasTag(std::string_view tag) const noexcept { return std::find(m_ctags.begin(), m_ctags.end(), tag) != m_ctags.end(); }
    void print() const;

    bool operator==(const Prefab& other) const noexcept {
        return m_name == other.getName();
    }

   private:
    std::string m_filepath{};
    std::string m_name{};
    std::vector<std::string> m_inherits{};
    std::unordered_map<std::string, nlohmann::json> m_components{};
    std::unordered_set<std::string> m_ctags{};

    void read(const json& j);
};
}