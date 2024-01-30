#include "assets/prefab.h"

#include "utils/timer.h"

namespace potatoengine::assets {

void Prefab::process_prototype(const std::string& name, const json& prototypeData,
                       const json& data) {
  std::vector<std::string> inherits;
  std::vector<std::string> ctags;
  std::unordered_map<std::string, json> components;

  if (prototypeData.contains("inherits")) {
    prototypeData.at("inherits").get_to(inherits);
    for (std::string_view father : inherits) {
      read(data.at(father), inherits, ctags, components);
    }
  }
  read(prototypeData, inherits, ctags,
       components); // child overrides parent if common definition exists

  m_prototypes.emplace(name, Prototype{.inherits = std::move(inherits),
                                       .ctags = std::move(ctags),
                                       .components = std::move(components)});
}

Prefab::Prefab(std::filesystem::path&& fp,
               std::vector<std::string>&& targetedPrototypes)
  : m_name(std::move(fp.filename().string())),
    m_filepath(std::move(fp.string())),
    m_targetedPrototypes(std::move(targetedPrototypes)) {
  // One prefab file can contain multiple prototypes and we target only a subset
  // of them
  std::ifstream f(fp);
  ENGINE_ASSERT(f.is_open(), "Failed to open prefab file!");
  ENGINE_ASSERT(f.peek() not_eq std::ifstream::traits_type::eof(),
                "Prefab file is empty!");
  json data = json::parse(f);
  f.close();

  if (m_targetedPrototypes == std::vector<std::string>{"*"}) {
    m_targetedPrototypes.clear();
    for (const auto& [name, prototypeData] : data.items()) {
      m_targetedPrototypes.emplace_back(name);
      process_prototype(name, prototypeData, data);
    }
  } else {
    for (const auto& [name, prototypeData] : data.items()) {
      if (std::find(m_targetedPrototypes.begin(), m_targetedPrototypes.end(),
                    name) == m_targetedPrototypes.end()) {
        continue;
      }
      process_prototype(name, prototypeData, data);
    }
  }
}

void Prefab::read(const json& data, std::vector<std::string>& inherits,
                  std::vector<std::string>& ctags,
                  std::unordered_map<std::string, json>& components) {
  if (data.contains("ctags")) {
    for (const json& c : data.at("ctags")) {
      ctags.emplace_back(c);
    }
  }

  if (data.contains("components")) {
    for (const auto& [cKey, cValue] : data.at("components").items()) {
      if (inherits.size() > 0) {
        std::erase_if(ctags, [&cKey](const std::string& c) { return c == cKey; });
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

const std::map<std::string, std::string, NumericComparator>& Prefab::getInfo() {
  if (not m_info.empty()) {
    return m_info;
  }

  m_info["Type"] = "Prefab";
  m_info["Filepath"] = m_filepath;
  for (const auto& [prototype_id, prototype_data] : m_prototypes) {
    m_info["Prototype " + prototype_id] = prototype_id;
  }

  return m_info;
}

const std::map<std::string, std::string, NumericComparator>&
Prefab::getTargetedPrototypeInfo(std::string_view prototypeID) {
  if (not m_prototypeInfo.empty() and
      m_prototypeInfo.contains(prototypeID.data())) {
    return m_prototypeInfo.at(prototypeID.data());
  }

  std::map<std::string, std::string, NumericComparator> m_info{};
  m_info["Name"] = prototypeID.data();
  for (uint32_t i = 0; i < m_prototypes.at(prototypeID.data()).inherits.size();
       ++i) {
    m_info["Inherits " + std::to_string(i)] =
      *std::next(m_prototypes.at(prototypeID.data()).inherits.begin(), i);
  }
  for (uint32_t i = 0; i < m_prototypes.at(prototypeID.data()).ctags.size(); ++i) {
    m_info["CTag " + std::to_string(i)] =
      *std::next(m_prototypes.at(prototypeID.data()).ctags.begin(), i);
  }
  uint32_t i = 0;
  for (const auto& [componentID, _] :
       m_prototypes.at(prototypeID.data()).components) {
    m_info["Component " + std::to_string(i++)] = componentID;
  }
  m_prototypeInfo[prototypeID.data()] = m_info;

  return m_prototypeInfo.at(prototypeID.data());
}

bool Prefab::operator==(const Asset& other) const {
  if (typeid(*this) not_eq typeid(other)) {
    ENGINE_ASSERT(false, "Cannot compare prefab with other asset type!");
  }
  const Prefab& otherPrefab = static_cast<const Prefab&>(other);
  return m_filepath == otherPrefab.m_filepath;
}
}