enable_language(CSharp)

file(GLOB HOOK_SOURCE_FILES
  *.cs
  Properties/*.cs
  *.snk)

add_library(VoMarionetteHook SHARED ${HOOK_SOURCE_FILES})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/VoMarionetteHook.snk
  ${CMAKE_CURRENT_BINARY_DIR}/VoMarionetteHook.snk COPYONLY)

# set_source_files_properties(${HOOK_SOURCE_FILES} PROPERTIES
#   COMPILE_FLAGS "/keyfile:VoMarionetteHook.snk")

set_target_properties(VoMarionetteHook PROPERTIES
  VS_GLOBAL_ROOTNAMESPACE ${PROJECT_NAME}
  DOTNET_TARGET_FRAMEWORK_VERSION "v4.5.2"
  VS_DOTNET_REFERENCES "System;System.Windows.Forms"
)
