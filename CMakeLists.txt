cmake_minimum_required(VERSION 3.16)
project(Grain LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automatically scan for sources
file(GLOB GRAIN_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)

# Create shared library
add_library(grain STATIC ${GRAIN_SOURCES})

if(MSVC)
    add_compile_options(/permissive- /W4)
else()
    # Clang/GCC need this explicit flag to use ::operator delete(void*, std::size_t),
    # instead of the unsized version ::operator delete(void*)
    add_compile_options(-fsized-deallocation)

    add_compile_options(-pedantic -Wall -Wextra)

    # Use ASan for memory leaks
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

# Public headers for install/include path
target_include_directories(grain PUBLIC ${PROJECT_SOURCE_DIR}/include)


# Tests
set(TEST_EXECUTABLES)
foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)

    # Create an executable per test source
    add_executable(test_${test_name} ${test_src})
    target_link_libraries(test_${test_name} PRIVATE grain)

    # Add to list
    list(APPEND TEST_EXECUTABLES test_${test_name})
endforeach()

# Run all tests
add_custom_target(test
    COMMENT "Running all tests..."
)

foreach(test_exe ${TEST_EXECUTABLES})
    add_custom_command(TARGET test
        POST_BUILD
        COMMAND MallocNanoZone=0 $<TARGET_FILE:${test_exe}>
        COMMENT "Running ${test_exe}"
    )
endforeach()
