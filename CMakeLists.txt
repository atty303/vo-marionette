cmake_minimum_required(VERSION 3.12.0)

project(injector VERSION 0.1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")

add_subdirectory(injector/controller)
add_subdirectory(injector/profiler)
add_subdirectory(injector/hook)

# --- paket
add_custom_target(nuget-restore
  COMMAND ${PROJECT_SOURCE_DIR}\\.paket\\paket.exe install
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
