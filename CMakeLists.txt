cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0167 NEW)

project(BattleTest VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vcpkg integration
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
endif()

# Find packages
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(asio CONFIG REQUIRED)

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES
    "main.cpp"
    "src/*.cpp"
    "src/*.h"
)

file(GLOB_RECURSE HEADERS
    "include/*.h"
    "include/*.hpp"
)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    Boost::system
    Boost::thread
    asio::asio
)

# Compile options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Debug/Release configurations
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_BUILD)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
endif()

# Tests (optional)
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# -----------------------------------------------------------------------------
# Linter and Formatter Setup
# -----------------------------------------------------------------------------

# This ensures clang-tidy knows about our compile definitions
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define all source and header files for formatting/linting
file(GLOB_RECURSE ALL_PROJECT_FILES
    "src/*.cpp"
    "src/*.h"
    "include/*.h"
    "include/*.hpp"
    "tests/*.cpp"
    "tests/*.h"
)

# Find clang-format
find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_PROJECT_FILES}
        COMMENT "Formatting C++ files with clang-format"
    )
else()
    message(STATUS "clang-format not found. 'format' target will not be available.")
endif()

# Find clang-tidy
find_program(CLANG_TIDY_EXE clang-tidy)
if(CLANG_TIDY_EXE)
    add_custom_target(
        lint
        COMMAND ${CLANG_TIDY_EXE}
                -p ${CMAKE_BINARY_DIR}
                --fix
                --quiet
                ${ALL_PROJECT_FILES}
        COMMENT "Linting C++ files with clang-tidy"
    )
else()
    message(STATUS "clang-tidy not found. 'lint' target will not be available.")
endif()