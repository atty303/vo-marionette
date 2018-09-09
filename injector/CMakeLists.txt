cmake_minimum_required(VERSION 3.0.0)

add_subdirectory(profiler)

include(CSharpUtilities)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

include(CTest)
enable_testing()

add_executable(controller controller/main.cs)

# --- NuGet
find_program(NUGET nuget)
add_custom_target(nuget-restore
  COMMAND ${NUGET} restore ${CMAKE_CURRENT_BINARY_DIR}/controller.sln
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Packages.config
  ${CMAKE_CURRENT_BINARY_DIR}/Packages.config COPYONLY)

#  VS_GLOBAL_ProjectGuid "{f84e584d-a422-3021-a20c-cec777413cae}"
set_target_properties(controller PROPERTIES
  VS_GLOBAL_ROOTNAMESPACE ${PROJECT_NAME}
  VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.5.2"
  VS_DOTNET_REFERENCES "System;System.Windows.Forms;CommandLine"
)

add_dependencies(controller nuget-restore)
# --- <<<

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
