# Project definition
cmake_minimum_required(VERSION 3.20)
project(potatocraft
    LANGUAGES CXX
    VERSION 1.0
)

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

set(BASEPATH "${CMAKE_SOURCE_DIR}")
include_directories("${BASEPATH}")

# Source files
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_subdirectory(vendor)

# Executable definition and properties
add_executable(
    potatocraft
    ${SOURCE_FILES}
)
include_directories(vendor/stb)
target_compile_definitions(${PROJECT_NAME} PRIVATE "STB_IMAGE_IMPLEMENTATION")
target_compile_definitions(${PROJECT_NAME} PRIVATE "STBI_FAILURE_USERMSG")
target_compile_definitions(${PROJECT_NAME} PRIVATE "STB_IMAGE_WRITE_IMPLEMENTATION")
target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
target_include_directories(${PROJECT_NAME} PRIVATE ${assimp_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${spdlog_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
option(ENTT_INCLUDE_NATVIS "Add EnTT natvis files to the EnTT target." ON)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

if (MSVC)
    # target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    # target_compile_options(${PROJECT_NAME} PRIVATE /experimental:module)
elseif(GCC OR CLANG)
    #target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Werror)
    target_compile_options(${PROJECT_NAME} PRIVATE -fmodules-ts)
endif()

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm glad imgui EnTT nlohmann_json::nlohmann_json assimp ${ASSIMP_LIBRARIES} spdlog ${CMAKE_DL_LIBS})

set(CMAKE_CXX_CLANG_TIDY
  clang-tidy;
  -header-filter=.;
  -checks=*;
  -warnings-as-errors=*;
)
