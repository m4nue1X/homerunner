
add_library(homerunner SHARED
  ${LIBHR_GENERATED_SOURCES}
  main.cpp
)

target_link_libraries(homerunner
  ${VNL_LIBRARIES}
)
