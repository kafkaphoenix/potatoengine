#pragma once

static constexpr const char SPONZA_SCENE[]{"sponza_scene"};
static constexpr const char SPONZA_SCENE_PATH[]{"assets/scenes/sponza_scene.json"};
static constexpr const char DABROVIC_SPONZA_SCENE[]{"dabrovic_sponza_scene"};
static constexpr const char DABROVIC_SPONZA_SCENE_PATH[]{"assets/scenes/dabrovic_sponza_scene.json"};
static constexpr const char CUBES_SCENE[]{"cubes_scene"};
static constexpr const char CUBES_SCENE_PATH[]{"assets/scenes/cubes_scene.json"};
static constexpr const char LOWPOLY_CITY_SCENE[]{"lowpoly_city_scene"};
static constexpr const char LOWPOLY_CITY_SCENE_PATH[]{"assets/scenes/lowpoly_city_scene.json"};
static constexpr const char VOXEL_TERRAIN_SCENE[]{"voxel_terrain_scene"};
static constexpr const char VOXEL_TERRAIN_SCENE_PATH[]{"assets/scenes/voxel_terrain_scene.json"};
static constexpr const char PLANE_TERRAIN_SCENE[]{"plane_terrain_scene"};
static constexpr const char PLANE_TERRAIN_SCENE_PATH[]{"assets/scenes/plane_terrain_scene.json"};
static constexpr const char PRIMITIVES_SCENE[]{"primitives_scene"};
static constexpr const char PRIMITIVES_SCENE_PATH[]{"assets/scenes/primitives_scene.json"};

static constexpr const char WINDOW_ICON_PATH[]{"assets/textures/icon.png"};
static constexpr const int DISPLAY_WIDTH{1280};
static constexpr const int DISPLAY_HEIGHT{720};
static constexpr const int DEPTH_BITS{24};
static constexpr const int REFRESH_RATE{60};   // only used in fullscreen mode
static constexpr const bool FULLSCREEN{};      // true: fullscreen, false: windowed
static constexpr const int PRIMARY_MONITOR{};  // 0: primary, 1: secondary
static constexpr const bool VSYNC{true};
static constexpr const bool RESIZABLE{true};
static constexpr const int OPENGL_MAJOR_VERSION{4};
static constexpr const int OPENGL_MINOR_VERSION{5};
static constexpr const char CURSOR_ICON_PATH[]{"assets/textures/cursor.png"};
static constexpr const int CURSOR_MODE{2};  // 0: normal, 1: hidden, 2: disabled
static constexpr const bool DEBUG_ENABLED{true};
static constexpr const int DEBUG_LEVEL{};  // 0: trace, 1: info, 2: warn, 3: error, 4: critical
static constexpr const bool DISPLAY_FPS{};
static constexpr const float CLEAR_COLOR[]{0.45f, 0.55f, 0.6f, 1.f};
static constexpr const float CLEAR_DEPTH{1.f};
static constexpr const char* DEFAULT_SCENE{PLANE_TERRAIN_SCENE};
static constexpr const char* DEFAULT_SCENE_PATH{PLANE_TERRAIN_SCENE_PATH};
