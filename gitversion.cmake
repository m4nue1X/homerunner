# Get working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")

# Get abbreviated commit hash
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")

# Get commit date
execute_process(
  COMMAND git log -1 --format=%aD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_AUTHOR_DATE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DGIT_AUTHOR_DATE="${GIT_AUTHOR_DATE}")
