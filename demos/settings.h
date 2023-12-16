#pragma once
#include <array>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace demos {
struct Settings {
    std::string appName = "Demos";
    std::string root = "..";
    std::string logFilePath = "logs/demos.log";

    std::string windowIconPath = "assets/textures/icon.png";
    int windowWidth = 1280;
    int windowHeight = 720;
    int depthBits = 24;
    int refreshRate = 60;    // only used in fullscreen mode
    bool fullscreen = false; // true: fullscreen, false: windowed
    int primaryMonitor = 0;  // 0: primary, 1: secondary
    bool vSync = true;
    bool resizable = true;
    int openglMajorVersion = 4;
    int openglMinorVersion = 5;

    std::string cursorIconPath = "assets/textures/cursor.png";
    int cursorMode = 2; // 0: normal, 1: hidden, 2: disabled

    bool debugEnabled = true;
    int debugLevel = 0; // 0: trace, 1: debug, 2: info, 3: warn, 4: error, 5: critical
    bool displayFPS = false;

    std::array<float, 4> clearColor = {0.45f, 0.55f, 0.6f, 1.f};
    float clearDepth = 1.f;

    std::string activeScene{"Empty"};
    std::string activeScenePath{"assets/scenes/empty_scene.json"};
    bool reloadScene = false;

    std::vector<const char*> scenes{"Sponza",        "Dabrovic Sponza", "Cubes", "Lowpoly City",
                                    "Plane Terrain", "Primitives",      "Empty"};
    std::vector<const char*> scenesPaths{
        "assets/scenes/sponza_scene.json",        "assets/scenes/dabrovic_sponza_scene.json",
        "assets/scenes/cubes_scene.json",         "assets/scenes/lowpoly_city_scene.json",
        "assets/scenes/plane_terrain_scene.json", "assets/scenes/primitives_scene.json",
        "assets/scenes/empty_scene.json"};
    std::vector<const char*> cursorModes{"Normal", "Hidden", "Disabled"};
    std::vector<const char*> debugLevels{"Trace", "Debug", "Info", "Warn", "Error", "Critical"};
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Settings, appName, root, logFilePath, windowIconPath, windowWidth, windowHeight,
                                   depthBits, refreshRate, fullscreen, primaryMonitor, vSync, resizable,
                                   openglMajorVersion, openglMinorVersion, cursorIconPath, cursorMode, debugEnabled,
                                   debugLevel, displayFPS, clearColor, clearDepth, activeScene, activeScenePath);
}