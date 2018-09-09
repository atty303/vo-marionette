enable_language(CSharp)

# --- NuGet
find_program(NUGET nuget)
add_custom_target(nuget-restore
  COMMAND ${NUGET} restore ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.sln
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Packages.config
  ${CMAKE_CURRENT_BINARY_DIR}/Packages.config COPYONLY)

file(GLOB CONTROLLER_SOURCE_FILES *.cs)

add_executable(controller ${CONTROLLER_SOURCE_FILES})
set_target_properties(controller PROPERTIES
  VS_GLOBAL_ROOTNAMESPACE ${PROJECT_NAME}
  VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.5.2"
  VS_DOTNET_REFERENCES "System;System.Windows.Forms;CommandLine"
)
add_dependencies(controller nuget-restore)
