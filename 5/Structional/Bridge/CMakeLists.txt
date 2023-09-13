cmake_minimum_required(VERSION 3.27.4)
set(EXECUTABLE_NAME bridge_example)
set(PROJECT_NAME bridge_example)

project (${PROJECT_NAME})

set(SOURCES
  src/main.cpp
  src/message_application.cpp
  src/message_platforms.cpp
)

add_executable(bridge_example ${SOURCES})

target_compile_features(bridge_example PRIVATE cxx_std_17)

target_include_directories(bridge_example
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)