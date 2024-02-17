option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" ON)

# Linters (Needs compilation database sadly no MSVC support yet, change to GCC to use)
# https://stackoverflow.com/questions/65544093/no-compile-commands-json-file-using-cmake-with-vs-code-on-windows
find_program(CLANG_TIDY NAMES "clang-tidy")

if(CLANG_TIDY AND ENABLE_CLANG_TIDY AND NOT GCC) # GCC doesn't support clang-tidy with precompiled headers
    file(COPY ${CMAKE_CURRENT_LIST_DIR}/../vendor/.clang-tidy DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

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