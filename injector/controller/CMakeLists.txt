enable_language(CSharp)

configure_file(paket.references
  ${CMAKE_CURRENT_BINARY_DIR}/paket.references COPYONLY)

file(GLOB CONTROLLER_SOURCE_FILES *.cs)

add_executable(controller ${CONTROLLER_SOURCE_FILES})
target_compile_options(controller
 PRIVATE "/langversion:6"
 PRIVATE "/unsafe")
set_target_properties(controller PROPERTIES
  VS_GLOBAL_ROOTNAMESPACE ${PROJECT_NAME}
  DOTNET_TARGET_FRAMEWORK_VERSION "v4.5.2"
  VS_DOTNET_REFERENCES "System;System.Runtime.Extensions;System.Windows.Forms;UIAutomationClient;UIAutomationTypes"
)
add_dependencies(controller nuget-restore)
