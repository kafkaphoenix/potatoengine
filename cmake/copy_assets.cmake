# https://stackoverflow.com/questions/42476765/copying-assets-directory-from-source-to-build-directory-in-cmake
# only update if something has changed
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets_command.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)