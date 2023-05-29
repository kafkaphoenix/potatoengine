# Project definition
cmake_minimum_required(VERSION 3.13...3.21)

project(potatocraft
LANGUAGES CXX
VERSION 1.0
)

# Generate the compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BASEPATH "${CMAKE_SOURCE_DIR}")
include_directories("${BASEPATH}")

# Source files
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/potatoengine/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/potatoengine/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/sandbox/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/sandbox/*.cpp"
)

add_subdirectory(vendor)

# Executable definition and properties
add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
)
target_compile_definitions(${PROJECT_NAME} PRIVATE "STB_IMAGE_IMPLEMENTATION")
target_compile_definitions(${PROJECT_NAME} PRIVATE "STBI_FAILURE_USERMSG")
target_compile_definitions(${PROJECT_NAME} PRIVATE "STB_IMAGE_WRITE_IMPLEMENTATION")
target_include_directories(${PROJECT_NAME} PRIVATE vendor/stb)
target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
target_include_directories(${PROJECT_NAME} PRIVATE ${assimp_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${spdlog_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${glm_SOURCE_DIR})
option(ENTT_INCLUDE_NATVIS ON)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_precompile_headers(${PROJECT_NAME} PRIVATE potatoengine/pch.h)

if (MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /EHs)
target_compile_options(${PROJECT_NAME} PRIVATE /external:I ${CMAKE_SOURCE_DIR}/build /W0) # Exclude from warnings
target_compile_options(${PROJECT_NAME} PRIVATE /experimental:module)
elseif(GCC OR CLANG)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Werror)
target_compile_options(-isystem ${CMAKE_SOURCE_DIR}/build)  # Exclude from warnings
target_compile_options(${PROJECT_NAME} PRIVATE -fmodules-ts)
endif()

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm glad imgui EnTT nlohmann_json::nlohmann_json assimp ${ASSIMP_LIBRARIES} spdlog ${CMAKE_DL_LIBS})

# Linters
# find_program(CLANG_TIDY NAMES "clang-tidy")

# if(CLANG_TIDY)
#     set(CLANG_TIDY_OPTIONS
#         "-checks=*"
#         "-header-filter=."
#         "-warnings-as-errors=*"
#         "-p=${CMAKE_SOURCE_DIR}"
#     )

#     add_custom_target(
#         run_clang_tidy
#         COMMAND ${CLANG_TIDY} ${CLANG_TIDY_OPTIONS} ${CMAKE_SOURCE_DIR}/potatoengine ${CMAKE_SOURCE_DIR}/sandbox
#         COMMENT "running clang-tidy static analysis"
#     )

#     add_dependencies(${PROJECT_NAME} run_clang_tidy)
# else()
#     message(WARNING "clang-tidy not found. Static analysis target will not be available")
# endif()

find_program(CPPCHECK NAMES "cppcheck")

if(CPPCHECK)
    set(CPPCHECK_OPTIONS
        "--enable=all"
        "--std=c++20"
        #"--check-config"
        "--platform=win64"
        "--suppress=missingIncludeSystem"
        "--suppress=missingInclude"
        "--template=gcc"
        #"--project=compile_commands.json"
        "-I${CMAKE_SOURCE_DIR}/build"
    )

    add_custom_target(
        run_cppcheck
        COMMAND ${CPPCHECK} ${CPPCHECK_OPTIONS} ${CMAKE_SOURCE_DIR}/potatoengine ${CMAKE_SOURCE_DIR}/sandbox
        COMMENT "running cppcheck static analysis"
    )
    add_dependencies(${PROJECT_NAME} run_cppcheck)
else()
    message(WARNING "cppcheck not found. Static analysis target will not be available")
endif()