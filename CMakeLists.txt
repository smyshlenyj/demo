cmake_minimum_required(VERSION 3.16)
project(calc VERSION 0.1.0 LANGUAGES CXX)

option(BUILD_TESTS "Build unit tests" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy checks" ON)

include(GNUInstallDirs)

# --------------------
# C++ settings
# --------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# --------------------
# Perf-friendly flags для RelWithDebInfo
# --------------------
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fno-omit-frame-pointer -fno-optimize-sibling-calls")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# --------------------
# clang-tidy (TARGET-BASED, NOT GLOBAL)
# --------------------
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY NAMES clang-tidy clang-tidy-18 clang-tidy-17)

    if(CLANG_TIDY)
        message(STATUS "clang-tidy found: ${CLANG_TIDY}")
    endif()
endif()

# --------------------
# Dependencies (FetchContent)
# --------------------
include(FetchContent)

# mathlib
FetchContent_Declare(
    mathlib
    GIT_REPOSITORY https://github.com/smyshlenyj/mathlib.git
    GIT_TAG main
)
FetchContent_MakeAvailable(mathlib)

# nlohmann/json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(spdlog)

# googletest
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Postgres + libpq
find_package(PostgreSQL REQUIRED)

if(PostgreSQL_FOUND)
    message(STATUS "PostgreSQL found: include=${PostgreSQL_INCLUDE_DIRS}, lib=${PostgreSQL_LIBRARIES}")
else()
    message(FATAL_ERROR "PostgreSQL not found")
endif()

# --------------------
# Main executable
# --------------------
add_executable(calc
    src/main.cpp
)

# my includes
target_include_directories(calc
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

# third party include as SYSTEM, in order to supress warnings
target_include_directories(calc SYSTEM PRIVATE
    ${mathlib_SOURCE_DIR}/include/
    ${json_SOURCE_DIR}/include/
    ${spdlog_SOURCE_DIR}/include/
    ${PostgreSQL_INCLUDE_DIRS} # путь к libpq-fe.h
)

# link libraries
target_link_libraries(calc
    PRIVATE
    math
    spdlog
    nlohmann_json::nlohmann_json
    PostgreSQL::PostgreSQL
)

# clang-tidy for my code not for third-party libs
if(CLANG_TIDY)
    set_target_properties(calc PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY}")
endif()

# --------------------
# Disable clang-tidy for tests and third-party
# --------------------
set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")

# --------------------
# Installation
# --------------------
install(TARGETS calc
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# --------------------
# Tests
# --------------------
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
