# potatoengine

Final project: C++ OpenGL engine for 3d and 2d game development.

## Engine Features

- Self contained engine, everything can be accessed just importing a header `engineAPI.h`
- Scene manager: Entity Component System engine oriented
- Render manager: OpenGL 4.5 API abstraction
- Window manager: Multi window/overlay support
- Assets manager: Caching and hot reloading of prefabs, shaders, textures, models and scenes
- States manager: State machine
- Settings manager: Persist your settings
- Debugger (Logger, Metrics, Dynamic settings, assets/entities/scene/states inspector)
- Event-driven (Mouse/Keyboard/Window/Application)
- Loading scenes and entity prototypes from json
- Perspective camera
- Model, Texture and Cubemaps loader
- Phong lighthing
- Framebuffer for post processing effects
- Triangle/Quad meshing terrain generation supporting texture, texture atlas and colors
- Noise generator for procedural terrain

# Demos

Inside the folder `demos` you can find several example scenes:

- Sponza
- Dabrovic sponza
- Lowpoly city
- Skycrapers city
- Trailer park
- Noise generated terrain and skybox
- Flat terrain with primitives and skybox
- Several cube instances with reflection, light, using a texture atlas and a skybox
- Flappy Bird game

## To compile and run
```
$ mkdir build ; cd build
$ cmake ../src
$ cmake --build . --target potatoengine
$ .\Debug\potatoengine.exe 
```

## How to use the engine in a personal project

- Once cloned the project create inside a folder where your game app logic will go, an example can be seen in `demos\`

## Planned features

- Expand camera class supporting more modes
- Profiler
- Serialization
- Scripting language
- Tests
- Editor Mode
- Engine wiki documentation

## Third party libraries

- [glfw](https://github.com/glfw/glfw): openGL application development framework
- [glad](https://github.com/Dav1dde/glad): openGL loader
- [EnTT](https://github.com/skypjack/entt): entity component system
- [stb](https://github.com/nothings/stb): image loading
- [glm](https://github.com/g-truc/glm): math operations
- [imGui](https://github.com/ocornut/imgui): ui framework
- [nlohmann_json](https://github.com/nlohmann/json): json processing
- [assimp](https://github.com/assimp/assimp): model loading
- [spdlog](https://github.com/gabime/spdlog): structured logging
- [FastNoiseLite](https://github.com/Auburn/FastNoiseLite): noise generation
- [freetype](https://github.com/freetype/freetype): font rendering
- [RmlUi](https://github.com/mikke89/RmlUi): ui framework