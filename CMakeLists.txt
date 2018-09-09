cmake_minimum_required(VERSION 3.0.0)

project(injector VERSION 0.1.0)

add_subdirectory(injector/controller)
add_subdirectory(injector/profiler)

# --- NuGet
find_program(NUGET nuget)
add_custom_target(nuget-restore
  COMMAND ${NUGET} restore ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.sln)

configure_file(${CMAKE_SOURCE_DIR}/injector/Packages.config
  ${CMAKE_BINARY_DIR}/Packages.config COPYONLY)

find_program(POWERSHELL_EXECUTABLE NAMES powershell)
