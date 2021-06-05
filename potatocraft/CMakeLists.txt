# Project definition
cmake_minimum_required(VERSION 3.20)
project(potatocraft
    LANGUAGES CXX
    VERSION 1.0
)

# Source files
FILE(GLOB SOURCE_FILES src/** src/World/** src/Render/**)

add_subdirectory(deps)

# Executable definition and properties
add_executable(
    potatocraft
    ${SOURCE_FILES}
)
target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm glad imgui EnTT nlohmann_json::nlohmann_json)