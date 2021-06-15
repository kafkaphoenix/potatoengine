# Project definition
cmake_minimum_required(VERSION 3.20)
project(potatocraft
    LANGUAGES CXX
    VERSION 1.0
)

set(BASEPATH "${CMAKE_SOURCE_DIR}")
include_directories("${BASEPATH}")

# Source files
FILE(GLOB SOURCE_FILES src/** src/world/** src/debug/** src/shader/** src/input/** src/camera/**)

add_subdirectory(deps)

# Executable definition and properties
add_executable(
    potatocraft
    ${SOURCE_FILES}
)
include_directories(deps/stb)
target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${assimp_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm glad imgui EnTT nlohmann_json::nlohmann_json assimp ${ASSIMP_LIBRARIES} ${CMAKE_DL_LIBS})