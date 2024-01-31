cmake_minimum_required(VERSION 3.21)

project(potatoengine
LANGUAGES CXX
VERSION 1.0
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" ON)
option(ENABLE_DEBUG "Enable debugging info" OFF)

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
target_compile_definitions(${PROJECT_NAME} PRIVATE "STBI_FAILURE_USERMSG")
target_include_directories(${PROJECT_NAME} PRIVATE vendor/stb)
target_include_directories(${PROJECT_NAME} PRIVATE vendor/fastnoiselite)
target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
target_include_directories(${PROJECT_NAME} PRIVATE ${assimp_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${spdlog_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_SOURCE_DIR}/include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

if (MSVC)
    # https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically?view=msvc-170
    target_compile_options(${PROJECT_NAME} PRIVATE /EHs /nologo /std:c++latest /Od /utf-8 /WX /Wall /wd4996) # /wd4996 is for deprecated functions msvc delete in the future
    target_compile_options(${PROJECT_NAME} PRIVATE /external:I ${CMAKE_SOURCE_DIR}/build /W0) # Exclude from warnings
elseif(GCC OR CLANG)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Werror -Wa -mbig-obj)
    target_compile_options(-isystem ${CMAKE_SOURCE_DIR}/build)  # Exclude from warnings
    target_compile_options(${PROJECT_NAME} PRIVATE -fmodules-ts)
endif()

# https://stackoverflow.com/questions/42476765/copying-assets-directory-from-source-to-build-directory-in-cmake
# only update if something has changed
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm glad imgui EnTT nlohmann_json::nlohmann_json assimp ${ASSIMP_LIBRARIES} spdlog RmlCore freetype ${CMAKE_DL_LIBS})

# Linters (Needs compilation database sadly no MSVC support yet, change to GCC to use)
# https://stackoverflow.com/questions/65544093/no-compile-commands-json-file-using-cmake-with-vs-code-on-windows
find_program(CLANG_TIDY NAMES "clang-tidy")

if(CLANG_TIDY AND ENABLE_CLANG_TIDY AND NOT GCC) # GCC doesn't support clang-tidy with precompiled headers
    add_custom_target(
        run_clang_tidy
        COMMAND ${CLANG_TIDY} ${CMAKE_SOURCE_DIR}/src/* ${CMAKE_SOURCE_DIR}/demos/* -p ${CMAKE_BINARY_DIR}/compile_commands.json
        COMMENT "running clang-tidy static analysis"
    )

    add_dependencies(${PROJECT_NAME} run_clang_tidy)
endif()

find_program(CPPCHECK NAMES "cppcheck")

if(CPPCHECK AND ENABLE_CPPCHECK)
    set(CPPCHECK_SOURCES
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/demos
    )

    set(CPPCHECK_OPTIONS
        "--template=gcc"
        "--platform=win64"
        "--std=c++2b"
        # "--project=${CMAKE_BINARY_DIR}\\compile_commands.json"
        "--enable=style,performance,portability"
        "--inline-suppr"
        "-I${CMAKE_SOURCE_DIR}\\build\\*"
        "-i${CMAKE_SOURCE_DIR}\\vendor\\*"
        "--suppress=*:*\\potatoengine\\vendor\\*:*"
        "--suppress=*:*\\potatoengine\\build\\*:*"
    )

    add_custom_target(
        run_cppcheck
        COMMAND ${CPPCHECK} ${CPPCHECK_OPTIONS} ${CPPCHECK_SOURCES}
        COMMENT "running cppcheck static analysis"
    )
    add_dependencies(${PROJECT_NAME} run_cppcheck)
endif()
