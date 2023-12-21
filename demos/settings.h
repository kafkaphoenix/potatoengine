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
    std::string backtraceLogFilePath = "logs/backtrace.log";

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
    bool windowInsideImgui = true; // it needs an opengl fbo to work
    bool fitToWindow = true;

    std::string cursorIconPath = "assets/textures/cursor.png";
    int cursorMode = 2; // 0: normal, 1: hidden, 2: disabled

    bool debugEnabled = true; // TODO use for something
    bool displayFPS = false;  // TODO implement with debugEnabled maybe?

    bool enableEngineLogger = true;
    bool enableAppLogger = true;
    // 0: trace, 1: debug, 2: info, 3: warning, 4: error, 5: critical
    int engineLogLevel = 0;
    int engineFlushLevel = 0;
    int appLogLevel = 0;
    int appFlushLevel = 0;
#ifdef DEBUG
    bool enableEngineBacktraceLogger = true;
    bool enableAppBacktraceLogger = true;
#else
    bool enableEngineBacktraceLogger = false;
    bool enableAppBacktraceLogger = false;
#endif

    std::array<float, 4> clearColor = {0.45f, 0.55f, 0.6f, 1.f};
    float clearDepth = 1.f;

    std::string activeScene{"Empty"};
    std::string activeScenePath{"assets/scenes/empty_scene.json"};
    bool reloadScene = false;

    std::vector<const char*> scenes{
      "Sponza", "Dabrovic Sponza", "Lowpoly City", "Skycrappers", "Room",
      "Cubes",  "Plane Terrain",   "Primitives",   "Empty",
    };
    std::vector<const char*> scenesPaths{
      "assets/scenes/sponza_scene.json",
      "assets/scenes/dabrovic_sponza_scene.json",
      "assets/scenes/lowpoly_city_scene.json",
      "assets/scenes/skycrappers.json",
      "assets/scenes/room.json",
      "assets/scenes/cubes_scene.json",
      "assets/scenes/plane_terrain_scene.json",
      "assets/scenes/primitives_scene.json",
      "assets/scenes/empty_scene.json",
    };
    std::vector<const char*> cursorModes{"Normal", "Hidden", "Disabled"};
    std::vector<const char*> logLevels{"Trace",   "Debug", "Info",
                                       "Warning", "Error", "Critical"};
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
  Settings, appName, root, logFilePath, backtraceLogFilePath, windowIconPath,
  windowWidth, windowHeight, depthBits, refreshRate, fullscreen, primaryMonitor,
  vSync, resizable, openglMajorVersion, openglMinorVersion, windowInsideImgui,
  fitToWindow, cursorIconPath, cursorMode, debugEnabled, displayFPS,
  enableEngineLogger, enableAppLogger, engineLogLevel, appLogLevel,
  engineFlushLevel, appFlushLevel, enableEngineBacktraceLogger,
  enableAppBacktraceLogger, clearColor, clearDepth, activeScene,
  activeScenePath);
}