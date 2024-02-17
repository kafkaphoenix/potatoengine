cmake_minimum_required(VERSION 3.21)


project(potatoengine
VERSION 1.0
HOMEPAGE_URL "https://github.com/kafkaphoenix/potatoengine"
DESCRIPTION "C++ OpenGL engine for 3d and 2d game development"
LANGUAGES CXX
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(ENABLE_DEBUG "Enable debugging info" OFF)

message(VERBOSE "*")
message(VERBOSE "* ${PROJECT_NAME} v${PROJECT_VERSION} (${CMAKE_BUILD_TYPE})")
message(VERBOSE "*")

# Source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/demos/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/demos/*.cpp"
)

add_subdirectory(vendor)

if (ENABLE_DEBUG)
    set(DEBUG ON)
else()
    set(DEBUG OFF)
endif()

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

# Executable definition and properties
add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE demos)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE "STBI_FAILURE_USERMSG")
target_include_directories(${PROJECT_NAME} PRIVATE vendor/stb)
target_include_directories(${PROJECT_NAME} PRIVATE vendor/fastnoiselite)
target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
target_include_directories(${PROJECT_NAME} PRIVATE ${assimp_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${spdlog_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE "RMLUI_STATIC_LIB")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm glad imgui EnTT nlohmann_json::nlohmann_json assimp ${ASSIMP_LIBRARIES} spdlog RmlCore freetype ${CMAKE_DL_LIBS})

include(${CMAKE_CURRENT_LIST_DIR}/cmake/warnings.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/copy_assets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/static_analysis.cmake)