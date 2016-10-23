
add_executable(hrnode
  main.cpp
)

target_link_libraries(hrnode
  dl
  pthread
  homerunner
)
