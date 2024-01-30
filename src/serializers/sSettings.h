#pragma once

#include <nlohmann/json.hpp>

#include "core/settingsManager.h"
#include "pch.h"

using json = nlohmann::json;

namespace potatoengine::serializers {

inline std::filesystem::path
get_default_roaming_path(std::string_view projectName) {
  auto path = std::filesystem::temp_directory_path()
                .parent_path()
                .parent_path()
                .parent_path();

  path /= "Roaming";

  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }

  path /= projectName.data();

  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }

  path /= "settings.json";

  return path;
}

inline void
save_settings(const std::unique_ptr<SettingsManager>& settings_manager,
              std::filesystem::path path) {
  json data = *settings_manager;

  std::ofstream file(path);
  ENGINE_ASSERT(file.is_open(), "Failed to open settings file!");

  file << data.dump(4);
  file.close();
}

inline std::unique_ptr<SettingsManager>
load_settings(std::string_view projectName) {
  std::unique_ptr<SettingsManager> settings_manager =
    std::make_unique<SettingsManager>();
  auto path = get_default_roaming_path(projectName);

  if (!std::filesystem::exists(path)) {
    settings_manager->appName = projectName.data();
    settings_manager->logFilePath = std::format("logs/{}.log", projectName);
    save_settings(settings_manager, path);
  } else {
    std::ifstream file(path);
    ENGINE_ASSERT(file.is_open(), "Failed to open settings file!");
    ENGINE_ASSERT(file.peek() not_eq std::ifstream::traits_type::eof(),
                  "Settings file is empty!");
    json data = json::parse(file);
    file.close();
    *settings_manager = data.get<SettingsManager>();
  }

  return settings_manager;
}

}