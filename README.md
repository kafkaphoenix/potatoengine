# potatoengine

C++ OpenGL engine for 3d and 2d game development

## Engine Features

- Self contained engine, everything can be accessed just importing a header `engineAPI.h`
- OpenGL 4.5 API abstraction
- Multi window/overlay support
- Entity Component System engine oriented
- Asset manager for caching and hot reloading prefabs, shaders, textures, models and scenes
- State machine
- Events (Mouse/Keyboard/Window/Application)
- Loading scenes and entity prototypes from json
- Debugger menu (Logger, Metrics, Dynamic settings, assets/entities inspector)
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
- Noise generated terrain and skybox
- Flat terrain with primitives and skybox
- Several cube instances with reflection, light, using a texture atlas and a skybox
- Voxel terrain (TODO)

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
- [imGui](https://github.com/ocornut/imgui): ui development framework
- [nlohmann_json](https://github.com/nlohmann/json): json processing
- [assimp](https://github.com/assimp/assimp): model loading
- [spdlog](https://github.com/gabime/spdlog): structured logging
- [FastNoiseLite](https://github.com/Auburn/FastNoiseLite): noise generation