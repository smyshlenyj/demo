cmake_minimum_required(VERSION 3.16)
project(calc VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs) # Provides CMAKE_INSTALL_*DIR variables for portable install paths

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Export compile commands for tools like clang-tidy, clangd, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable compiler warnings for GCC / Clang
add_compile_options(-Wall -Wextra -Wpedantic)

# clang-tidy setup
option(ENABLE_CLANG_TIDY "Enable clang-tidy checks" ON)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY NAMES clang-tidy clang-tidy-18 clang-tidy-17)

    if(CLANG_TIDY)
        message(STATUS "clang-tidy found: ${CLANG_TIDY}")

        # Apply clang-tidy to all C++ targets created after this point
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY}")
    endif()
endif()

# Include FetchContent module to download external libraries
include(FetchContent)

# Declare mathlib as an external dependency
FetchContent_Declare(
    mathlib
    GIT_REPOSITORY https://github.com/smyshlenyj/mathlib.git # URL of the mathlib repository
    GIT_TAG main # Branch or tag to use
)

# Download and make mathlib available as a CMake target
FetchContent_MakeAvailable(mathlib)

# Create main executable
add_executable(calc src/main.cpp src/runner.cpp)

# Link with mathlib
target_link_libraries(calc PRIVATE math)

# Install executable
install(TARGETS calc
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
