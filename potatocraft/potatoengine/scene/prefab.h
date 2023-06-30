#pragma once
#include <nlohmann/json.hpp>

#include "potatoengine/pch.h"
using json = nlohmann::json;

namespace potatoengine {
class Prefab {
   public:
    Prefab(const std::filesystem::path& fp, const std::string& target);
    ~Prefab();

    std::string_view getName() const { return m_name; }
    std::string_view getFilepath() const noexcept { return m_filepath; }
    const std::vector<std::string>& getInherit() const noexcept { return m_inherit; }
    using ftypes = std::variant<std::string, int, float, bool>;
    const std::unordered_map<std::string, std::variant<ftypes, std::map<std::string, ftypes>>>& getComponents() const { return m_components; }
    const std::vector<std::string>& getCTags() const noexcept { return m_ctags; }
    void print() const;

    bool operator==(const Prefab& other) const {
        return m_name == other.getName();
    }

   private:
    std::string m_filepath;
    std::string m_name;
    std::vector<std::string> m_inherit;
    std::unordered_map<std::string, std::variant<ftypes, std::map<std::string, ftypes>>> m_components;
    std::vector<std::string> m_ctags;

    void read(const json& data);
};
}