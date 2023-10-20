#pragma once

#include <nlohmann/json.hpp>

#include "potatoengine/pch.h"
using json = nlohmann::json;

struct PrefabData {
    std::unordered_set<std::string> inherits{};
    std::unordered_set<std::string> ctags{};
    std::unordered_map<std::string, json> components{};
};

namespace potatoengine {
class Prefab {
   public:
    Prefab(std::filesystem::path&& fp, std::unordered_set<std::string>&& targets);
    Prefab& operator=(const Prefab&) = delete;

    std::string_view getFilepath() const noexcept { return m_filepath; }
    const std::unordered_set<std::string>& getInherits(std::string_view id) const noexcept { return m_prefabs.at(id.data()).inherits; }
    const std::unordered_set<std::string>& getCTags(std::string_view id) const noexcept { return m_prefabs.at(id.data()).ctags; }
    const std::unordered_map<std::string, json>& getComponents(std::string_view id) const noexcept { return m_prefabs.at(id.data()).components; }
    const std::unordered_set<std::string>& getTargets() const noexcept { return m_targets; }
    void print() const;

    bool operator==(const Prefab& other) const noexcept {
        return m_filepath == other.m_filepath;
    }

   private:
    std::string m_filepath{};
    std::unordered_set<std::string> m_targets{};
    std::unordered_map<std::string, PrefabData> m_prefabs{};

    void read(const json& j, std::unordered_set<std::string>& inherits, std::unordered_set<std::string>& ctags, std::unordered_map<std::string, json>& components);
};
}