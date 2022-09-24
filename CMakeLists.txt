cmake_minimum_required(VERSION 3.20)
project(
    potatocraft
    VERSION 0.0.1
    DESCRIPTION "YAMC - Yet Another Minecraft Clone"
    HOMEPAGE_URL "https://github.com/kafkaphoenix/potatocraft"
    LANGUAGES CXX
)

set(BASEPATH "${CMAKE_SOURCE_DIR}")
include_directories("${BASEPATH}")

# Source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp *.hpp)
add_subdirectory(deps)

# Executable definition and properties
add_executable(
    potatocraft
    ${SOURCE_FILES}
)
include_directories(deps/stb)
target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${assimp_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${spdlog_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pcpch.hpp)

if (MSVC)
    # target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_compile_options(${PROJECT_NAME} PRIVATE /experimental:module)
elseif(GCC OR CLANG)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(${PROJECT_NAME} PRIVATE -fmodules-ts)
endif()

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm glad imgui EnTT nlohmann_json::nlohmann_json assimp ${ASSIMP_LIBRARIES} spdlog ${CMAKE_DL_LIBS})