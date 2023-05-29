include(FetchContent)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
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
  option(GLFW_BUILD_DOCS OFF)
  option(GLFW_BUILD_TESTS OFF)
  option(GLFW_BUILD_EXAMPLES OFF)
  option(GLFW_INSTALL OFF)
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
  option(GLM_TEST_ENABLE OFF)
	add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.89.5
)

FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR}/ ${imgui_SOURCE_DIR}/backends)
    file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp)
    file(GLOB IMGUI_HEADERS ${imgui_SOURCE_DIR}/*.h)
    add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_SOURCES} ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
    add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
    target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})
    target_link_libraries(imgui ${OPENGL_LIBRARIES} glfw glad)
endif ()

FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG v3.11.1
)

FetchContent_MakeAvailable(entt)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE
  GIT_TAG v3.10.0
)

FetchContent_MakeAvailable(json)

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v5.2.5
)

FetchContent_GetProperties(assimp)
if(NOT assimp_POPULATED)
	FetchContent_Populate(assimp)
  option(BUILD_SHARED_LIBS OFF)
  option(ASSIMP_BUILD_TESTS OFF)
  option(ASSIMP_INSTALL OFF)
  option(ASSIMP_WARNINGS_AS_ERRORS ON)
  option(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
  option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
	add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
endif()

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.11.0)

FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
  FetchContent_Populate(spdlog)
  option(SPDLOG_BUILD_EXAMPLE OFF)
  option(SPDLOG_INSTALL OFF)
  add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()