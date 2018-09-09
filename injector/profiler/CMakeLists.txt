set(CMAKE_CONFIGURATION_TYPES Debug Release)

enable_language(CXX)

set(MIDL_FILE
  ${CMAKE_CURRENT_LIST_DIR}/VoMarionetteProfiler.idl
)
get_FILENAME_COMPONENT(_MIDL_OUT_FILE ${MIDL_FILE} NAME_WE)
set(MIDL_HEADER_FILE ${CMAKE_CURRENT_BINARY_DIR}/${_MIDL_OUT_FILE}_h.h)
set(MIDL_IID_FILE ${CMAKE_CURRENT_BINARY_DIR}/${_MIDL_OUT_FILE}.c)

set(MIDL_OUTPUT
  ${MIDL_HEADER_FILE}
  ${MIDL_IID_FILE}
)
#  COMMAND midl /h IFace.h /iid GUIDS.c /proxy Proxy.c /dlldata ProxyDll.c ${MIDL_FILE}
add_custom_command(
  OUTPUT ${MIDL_OUTPUT}
  COMMAND midl /h ${MIDL_HEADER_FILE} /iid ${MIDL_IID_FILE} ${MIDL_FILE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${MIDL_FILE}
  VERBATIM
)
add_custom_target(midl-cmplr
  DEPENDS ${MIDL_OUTPUT}
)

set_source_files_properties(
  ${MIDL_OUTPUT}
    PROPERTIES
      GENERATED TRUE
)

add_library(profiler SHARED
  main.cpp
  factory.h
  ${MIDL_OUTPUT}
  )
add_definitions(-DDLL_EXPORT -I${CMAKE_CURRENT_BINARY_DIR})
add_dependencies(profiler
  midl-cmplr
)
