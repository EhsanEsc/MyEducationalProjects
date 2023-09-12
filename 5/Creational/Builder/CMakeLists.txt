cmake_minimum_required(VERSION 3.27.4)
set(EXECUTABLE_NAME builder_example)
set(PROJECT_NAME builder_example)

project (${PROJECT_NAME})

set(SOURCES
  src/main.cpp
  src/italian_builder.cpp
  src/stats_builder.cpp
  src/chef.cpp
)
add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${EXECUTABLE_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)