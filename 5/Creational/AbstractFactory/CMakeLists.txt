cmake_minimum_required(VERSION 3.27.4)
set(EXECUTABLE_NAME abstract_factory_example)
set(PROJECT_NAME abstract_factory_example)

project (${PROJECT_NAME})

set(SOURCES
  src/main.cpp
  src/car.cpp
  src/motorcycle.cpp
  src/saipa_factory.cpp
)
add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${EXECUTABLE_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)