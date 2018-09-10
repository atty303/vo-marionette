enable_language(CSharp)

# add_custom_target(controller-restore
#   COMMAND ${POWERSHELL_EXECUTABLE} -Command Uninstall-Package CommandLineParser -ProjectName controller
#   COMMAND ${POWERSHELL_EXECUTABLE} -Command Install-Package CommandLineParser -ProjectName controller)

file(GLOB CONTROLLER_SOURCE_FILES *.cs)

add_executable(controller ${CONTROLLER_SOURCE_FILES})
set_target_properties(controller PROPERTIES
  VS_GLOBAL_ROOTNAMESPACE ${PROJECT_NAME}
  DOTNET_TARGET_FRAMEWORK_VERSION "v4.5.2"
  VS_DOTNET_REFERENCES "System;System.Windows.Forms;CommandLine"
)
# add_dependencies(controller controller-restore)

