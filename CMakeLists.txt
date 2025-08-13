cmake_minimum_required(VERSION 3.16)
project(Grain LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automatically scan for sources
file(GLOB GRAIN_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Create shared library
add_library(grain STATIC ${GRAIN_SOURCES})

# Public headers for install/include path
target_include_directories(grain PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Test executable
add_executable(grain_test tests/main.cpp)
target_link_libraries(grain_test PRIVATE grain)

# Cross-platform test target
add_custom_target(test
  COMMAND grain_test
  DEPENDS grain_test
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
