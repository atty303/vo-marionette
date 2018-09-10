cmake_minimum_required(VERSION 3.12.0)

project(injector VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")

add_subdirectory(injector/controller)
add_subdirectory(injector/profiler)

# --- NuGet
find_program(NUGET nuget)
add_custom_target(nuget-restore
  COMMAND ${NUGET} restore ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.sln)

configure_file(${CMAKE_SOURCE_DIR}/injector/Packages.config
  ${CMAKE_BINARY_DIR}/Packages.config COPYONLY)

find_program(POWERSHELL_EXECUTABLE NAMES powershell)

