include(FetchContent)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36 # only tag that works
)

FetchContent_MakeAvailable(glad)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
	FetchContent_Populate(glfw)
  set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)
  set(GLFW_INSTALL OFF CACHE INTERNAL "" FORCE)
	add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG		   0.9.9.8
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
	FetchContent_Populate(glm)
  set(GLM_TEST_ENABLE OFF CACHE INTERNAL "" FORCE)
	add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.90
)

FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR}/ ${imgui_SOURCE_DIR}/backends)
    set(IMGUI_MISC_CPP_DIR ${imgui_SOURCE_DIR}/misc/cpp)
    set(IMGUI_INCLUDE_DIR ${IMGUI_INCLUDE_DIR} ${IMGUI_MISC_CPP_DIR})
    file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp ${IMGUI_MISC_CPP_DIR}/*.cpp)
    file(GLOB IMGUI_HEADERS ${imgui_SOURCE_DIR}/*.h ${IMGUI_MISC_CPP_DIR}/*.h)
    add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_SOURCES} ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
    add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
    target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})
    target_link_libraries(imgui ${OPENGL_LIBRARIES} glfw glad)
endif ()

FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG v3.12.2
)

FetchContent_GetProperties(entt)
if(NOT entt_POPULATED)
  FetchContent_Populate(entt)
  set(ENTT_INCLUDE_NATVIS ON CACHE INTERNAL "" FORCE)
  add_subdirectory(${entt_SOURCE_DIR} ${entt_BINARY_DIR})
endif()

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/fabio-polystream/nlohmann_json_cmake_fetchcontent.git
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE
  GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(json)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v5.3.1
)

FetchContent_GetProperties(assimp)
if(NOT assimp_POPULATED)
	FetchContent_Populate(assimp)
  set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
  set(ASSIMP_BUILD_ZLIB ON CACHE INTERNAL "" FORCE)
  set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
  set(ASSIMP_INSTALL OFF CACHE INTERNAL "" FORCE)
  set(ASSIMP_WARNINGS_AS_ERRORS ON CACHE INTERNAL "" FORCE)
  set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE INTERNAL "" FORCE)
  set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE INTERNAL "" FORCE)
  set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE INTERNAL "" FORCE)
	add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
endif()

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.12.0
)

FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
  FetchContent_Populate(spdlog)
  set(SPDLOG_BUILD_EXAMPLE OFF CACHE INTERNAL "" FORCE)
  set(SPDLOG_INSTALL OFF CACHE INTERNAL "" FORCE)
  set(SPDLOG_USE_STD_FORMAT ON CACHE INTERNAL "" FORCE)
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
  date_src
  GIT_REPOSITORY https://github.com/HowardHinnant/date.git
  GIT_TAG        v3.0.1
)

FetchContent_MakeAvailable(date_src)