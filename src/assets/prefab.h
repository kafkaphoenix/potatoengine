#pragma once

#include <nlohmann/json.hpp>

#include "assets/asset.h"
#include "pch.h"
#include "utils/numericComparator.h"

using json = nlohmann::json;

struct Prototype {
    std::unordered_set<std::string> inherits{};
    std::unordered_set<std::string> ctags{};
    std::unordered_map<std::string, json> components{};
};

namespace potatoengine::assets {
class Prefab : public Asset {
  public:
    Prefab(std::filesystem::path&& fp,
           std::unordered_set<std::string>&& targetedPrototypes);

    virtual const std::map<std::string, std::string, NumericComparator>&
    getInfo() override final;
    const std::map<std::string, std::string, NumericComparator>&
    getTargetedPrototypeInfo(std::string_view prototypeID);

    const std::unordered_set<std::string>&
    getInherits(std::string_view prototypeID) const {
      return m_prototypes.at(prototypeID.data()).inherits;
    }

    const std::unordered_set<std::string>&
    getCTags(std::string_view prototypeID) const {
      return m_prototypes.at(prototypeID.data()).ctags;
    }

    const std::unordered_map<std::string, json>&
    getComponents(std::string_view prototypeID) const {
      return m_prototypes.at(prototypeID.data()).components;
    }

    const std::unordered_set<std::string>&
    getTargetedPrototypes() const noexcept {
      return m_targetedPrototypes;
    }

    std::string_view getName() const noexcept { return m_name; }

    const std::unordered_map<std::string, Prototype>& getPrototypes() const {
      return m_prototypes;
    }

    virtual bool operator==(const Asset& other) const override final;

  private:
    std::string m_name{};
    std::string m_filepath{};
    std::unordered_set<std::string> m_targetedPrototypes{};
    std::unordered_map<std::string, Prototype> m_prototypes{};

    std::map<std::string, std::string, NumericComparator> m_info{};
    std::map<std::string, std::map<std::string, std::string, NumericComparator>,
             NumericComparator>
      m_prototypeInfo{};

    void read(const json& j, std::unordered_set<std::string>& inherits,
              std::unordered_set<std::string>& ctags,
              std::unordered_map<std::string, json>& components);

    void process_prototype(const std::string& name, const json& prototypeData,
                       const json& data);
};
}