

execute_process(
  COMMAND find ${CMAKE_CURRENT_SOURCE_DIR}/../generated/src -name *.cxx
  OUTPUT_VARIABLE FINDSRCFILES_OUTPUT
  RESULT_VARIABLE FINDSRCFILES_RESULT
)
string(REPLACE "\n" ";" HR_GEN_SOURCES ${FINDSRCFILES_OUTPUT})

if(!"${FINDSRCFILES_RESULT}" EQUAL "0")
  message(FATAL_ERROR "  failed to find generated source files!")
endif(!"${FINDSRCFILES_RESULT}" EQUAL "0")

add_library(hr SHARED
  ${HR_GEN_SOURCES}
  main.cpp
)

target_link_libraries(hr
  ${VNL_LIBS}
)
