# potatocraft
YAMC 'Yet Another Minecraft Clone'

## To compile and run
```
$ mkdir build ; cd build
$ cmake ../potatocraft
$ cmake --build . --target potatocraft
$ .\Debug\potatocraft.exe 
```

## Engine Features

- Self contained engine, everything can be accessed just importing the engine api `potatoengine\engineAPI.h`
- ECS powered by [EnTT](https://github.com/skypjack/entt)
- Stateful loading from json (Scenes and entities)
- Application state machine
- Events
- Asset manager for caching and hot reloading prefabs, shaders, textures, models and scenes
- Structured logger
- Multi window/overlay support
- OpenGL 4.5 API abstraction
- GLFW API abstraction
- Perspective camera
- Model, Texture and Cubemaps loader
- Phong lighthing
- Framebuffer for post processing effects

## How to use the engine in a personal project

- Create a folder in `\potatocraft\` inside you need to define the app.cpp and your game states with the game logic,
an example can be seen in `potatocraft\sandbox\`

## Planned features

- Debugging menu
- Profiler
- Metrics
- Serialization
- Expand camera class supporting more modes
- Scripting language
- Tests
- Engine wiki documentation

## Third party libraries

- [glfw](https://github.com/glfw/glfw): OpenGL/Vulkan application development framework
- [glad](https://github.com/Dav1dde/glad): OpenGL/Vulkan loader
- [EnTT](https://github.com/skypjack/entt): ECS
- [stb](https://github.com/nothings/stb): image loading
- [glm](https://github.com/g-truc/glm): math operations
- [imgui](https://github.com/ocornut/imgui): debugging
- [nlohmann_json](https://github.com/nlohmann/json): json processing
- [assimp](https://github.com/assimp/assimp): model loading
- [spdlog](https://github.com/gabime/spdlog): structured logging