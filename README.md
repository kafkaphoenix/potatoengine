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
- Game states
- ECS powered by [Entt](https://github.com/skypjack/entt)
- Stateful loading from json
- Events
- Asset manager for caching and hot reloading prefabs, shaders, textures and others
- Structured logger
- Supports several windows or overlays
- Renderer API abstraction over OpenGL 4.6
- Input API abstraction over GLFW
- 3d camera

## How to use the engine in a personal project

- Create a folder in `\potatocraft\` inside you need to define the app.cpp and your game states with the game logic,
an example can be seen in `potatocraft\sandbox\`

## Planned features

- FBO
- More shaders examples
- Model loader
- Expanded sandbox game example
- Debugging menu
- Commander
- Metrics
- Serialization
- Profiler
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