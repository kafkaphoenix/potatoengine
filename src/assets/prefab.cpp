#include "assets/prefab.h"

#include "utils/timer.h"

namespace potatoengine {
Prefab::Prefab(std::filesystem::path&& fp, std::unordered_set<std::string>&& targetedPrototypes)
    : m_filepath(std::move(fp.string())), m_targetedPrototypes(std::move(targetedPrototypes)) {
  // One prefab file can contain multiple prototypes and we target only a subset of them
  std::ifstream f(fp);
  ENGINE_ASSERT(f.is_open(), "Failed to open prefab file!");
  ENGINE_ASSERT(f.peek() != std::ifstream::traits_type::eof(), "Prefab file is empty!");
  json data = json::parse(f);
  f.close();

  for (const auto& [name, prototypeData] : data.items()) {
    if (not m_targetedPrototypes.contains(name)) {
      continue;
    }
    std::unordered_set<std::string> inherits;
    std::unordered_set<std::string> ctags;
    std::unordered_map<std::string, json> components;

    if (prototypeData.contains("inherit")) {
      prototypeData.at("inherit").get_to(inherits);
      for (std::string_view father : inherits) {
        read(data.at(father), inherits, ctags, components);
      }
    }
    read(prototypeData, inherits, ctags, components); // child overrides parent if common definition exists

    m_prototypes.emplace(
        name,
        Prototype{.inherits = std::move(inherits), .ctags = std::move(ctags), .components = std::move(components)});
  }
}

void Prefab::read(const json& data, std::unordered_set<std::string>& inherits, std::unordered_set<std::string>& ctags,
                  std::unordered_map<std::string, json>& components) {
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

const std::map<std::string, std::string, NumericComparator>& Prefab::getInfo() {
  if (not m_info.empty()) {
    return m_info;
  }

  m_info["Type"] = "Prefab";
  m_info["Filepath"] = m_filepath;
  for (int i = 0; i < m_targetedPrototypes.size(); ++i) {
    m_info["Targeted Prototype " + std::to_string(i)] = *std::next(m_targetedPrototypes.begin(), i);
  }

  return m_info;
}

const std::map<std::string, std::string, NumericComparator>&
Prefab::getTargetedPrototypeInfo(std::string_view prototypeID) {
  if (not m_prototypeInfo.empty() and m_prototypeInfo.contains(prototypeID.data())) {
    return m_prototypeInfo.at(prototypeID.data());
  }

  std::map<std::string, std::string, NumericComparator> m_info{};
  m_info["Name"] = prototypeID.data();
  for (int i = 0; i < m_prototypes.at(prototypeID.data()).inherits.size(); ++i) {
    m_info["Inherits " + std::to_string(i)] = *std::next(m_prototypes.at(prototypeID.data()).inherits.begin(), i);
  }
  for (int i = 0; i < m_prototypes.at(prototypeID.data()).ctags.size(); ++i) {
    m_info["CTag " + std::to_string(i)] = *std::next(m_prototypes.at(prototypeID.data()).ctags.begin(), i);
  }
  int i = 0;
  for (const auto& [componentID, _] : m_prototypes.at(prototypeID.data()).components) {
    m_info["Component " + std::to_string(i++)] = componentID;
  }
  m_prototypeInfo[prototypeID.data()] = m_info;

  return m_prototypeInfo.at(prototypeID.data());
}
}