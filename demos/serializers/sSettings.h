#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

#include "settings.h"
using json = nlohmann::json;

namespace demos::serializers {

inline std::filesystem::path load_path() {
  auto path = std::filesystem::temp_directory_path().parent_path().parent_path().parent_path();

  path /= "Roaming";

  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }

  path /= "Potatoengine"; // Change this to your app name

  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }

  path /= "settings.json";

  return path;
}

inline void save_settings(const Settings& settings, std::filesystem::path path = load_path()) {
  json data = settings;

  std::ofstream file(path);
  APP_ASSERT(file.is_open(), "Failed to open settings file!");

  file << data.dump(4);
  file.close();
}

inline Settings load_settings() {
  Settings settings;
  auto path = load_path();

  if (!std::filesystem::exists(path)) {
    save_settings(settings, path);
  } else {
    std::ifstream file(path);
    APP_ASSERT(file.is_open(), "Failed to open settings file!");
    APP_ASSERT(file.peek() != std::ifstream::traits_type::eof(), "Settings file is empty!");
    json data = json::parse(file);
    file.close();

    settings = data.get<Settings>();
  }

  return settings;
}

}