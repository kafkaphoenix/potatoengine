if (MSVC)
    # https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically?view=msvc-170
    target_compile_options(${PROJECT_NAME} PRIVATE /EHs /nologo /std:c++latest /Od /utf-8 /WX /Wall /wd4996) # /wd4996 is for deprecated functions msvc delete in the future
    target_compile_options(${PROJECT_NAME} PRIVATE /external:I ${CMAKE_SOURCE_DIR}/build /W0) # Exclude from warnings
elseif(GCC OR CLANG)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Werror -Wa -mbig-obj)
    target_compile_options(-isystem ${CMAKE_SOURCE_DIR}/build)  # Exclude from warnings
endif()
