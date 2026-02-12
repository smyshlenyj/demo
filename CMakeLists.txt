cmake_minimum_required(VERSION 3.16)
project(calc VERSION 0.1.0 LANGUAGES CXX)

option(BUILD_TESTS "Build unit tests" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------
# Build type
# --------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic)

# --------------------
# clang-tidy
# --------------------
option(ENABLE_CLANG_TIDY "Enable clang-tidy checks" ON)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY NAMES clang-tidy clang-tidy-18 clang-tidy-17)

    if(CLANG_TIDY)
        message(STATUS "clang-tidy found: ${CLANG_TIDY}")
    endif()
endif()

# --------------------
# FetchContent dependencies
# --------------------
include(FetchContent)

# mathlib
FetchContent_Declare(
    mathlib
    GIT_REPOSITORY https://github.com/smyshlenyj/mathlib.git
    GIT_TAG main
)
FetchContent_MakeAvailable(mathlib)

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

# --------------------
# PostgreSQL
# --------------------
find_package(PostgreSQL REQUIRED)

if(PostgreSQL_FOUND)
    message(STATUS "PostgreSQL found: include=${PostgreSQL_INCLUDE_DIRS}, lib=${PostgreSQL_LIBRARIES}")
else()
    message(FATAL_ERROR "PostgreSQL not found")
endif()

# --------------------
# Protobuf / gRPC (local built)
# --------------------
set(Protobuf_PROTOC_EXECUTABLE /usr/local/bin/protoc)
set(GRPC_CPP_PLUGIN_EXECUTABLE /usr/local/bin/grpc_cpp_plugin)

find_package(Protobuf REQUIRED)
message(STATUS "Protobuf include: ${Protobuf_INCLUDE_DIRS}")
find_package(gRPC CONFIG REQUIRED)

# --------------------
# Generated Proto Directory
# --------------------
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

set(PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/proto/calc.proto)
set(GENERATED_PROTO_SRCS "")
set(GENERATED_PROTO_HDRS "")

foreach(FIL ${PROTO_FILES})
    get_filename_component(FIL_WE ${FIL} NAME_WE)
    set(SRC "${GENERATED_DIR}/${FIL_WE}.pb.cc")
    set(HDR "${GENERATED_DIR}/${FIL_WE}.pb.h")
    set(GRPC_SRC "${GENERATED_DIR}/${FIL_WE}.grpc.pb.cc")
    set(GRPC_HDR "${GENERATED_DIR}/${FIL_WE}.grpc.pb.h")

    add_custom_command(
        OUTPUT ${SRC} ${HDR} ${GRPC_SRC} ${GRPC_HDR}
        COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        ARGS --cpp_out=${GENERATED_DIR}
        --grpc_out=${GENERATED_DIR}
        --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_EXECUTABLE}
        -I ${CMAKE_CURRENT_SOURCE_DIR}/proto
        ${FIL}
        DEPENDS ${FIL}
        COMMENT "Generating gRPC/protobuf code from ${FIL}"
        VERBATIM
    )

    list(APPEND GENERATED_PROTO_SRCS ${SRC} ${GRPC_SRC})
    list(APPEND GENERATED_PROTO_HDRS ${HDR} ${GRPC_HDR})
endforeach()

add_custom_target(proto_gen ALL
    DEPENDS ${GENERATED_PROTO_SRCS} ${GENERATED_PROTO_HDRS}
)

set(GENERATED_ALL_SRCS ${GENERATED_PROTO_SRCS} ${GENERATED_PROTO_HDRS})

# --------------------
# Build oir lib
# --------------------
add_library(calc_core OBJECT
    src/main.cpp
)

# Add sourcesfrom src
target_include_directories(calc_core
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/utils
    ${GENERATED_DIR} # proto
    SYSTEM PUBLIC
    ${spdlog_SOURCE_DIR}/include
    ${mathlib_SOURCE_DIR}/include
    ${PostgreSQL_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
)

# calc_core depends on proto_gen add_dependencies
add_dependencies(calc_core proto_gen)

# TSan turn on for calc_core ONLY
target_compile_options(calc_core PRIVATE -fsanitize=thread -g)

# --------------------
# Executable
# --------------------
add_executable(calc)
add_dependencies(calc proto_gen)

# Add object files from calc_core + generated proto files
target_sources(calc PRIVATE $<TARGET_OBJECTS:calc_core> ${GENERATED_PROTO_SRCS})

target_include_directories(calc SYSTEM PRIVATE
    ${mathlib_SOURCE_DIR}/include/
    ${spdlog_SOURCE_DIR}/include/
    ${PostgreSQL_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
    ${GENERATED_DIR}
)

target_link_libraries(calc
    PRIVATE
    math
    spdlog
    PostgreSQL::PostgreSQL
    protobuf::libprotobuf
    gRPC::grpc++
    pthread
)

# --- Tsan forlinking ---
target_link_options(calc PRIVATE -fsanitize=thread)

# --------------------
# Surpess warnings for generated proto files
# --------------------
foreach(proto_file IN LISTS GENERATED_ALL_SRCS)
    set_source_files_properties(${proto_file} PROPERTIES COMPILE_FLAGS "-w")
    set_source_files_properties(${proto_file} PROPERTIES CXX_CLANG_TIDY "")
endforeach()

# --------------------
# clang-tidy
# --------------------
if(CLANG_TIDY AND CLANG_TIDY_FOUND)
    set_target_properties(calc PROPERTIES
        CXX_CLANG_TIDY "clang-tidy;-header-filter='^${PROJECT_SOURCE_DIR}/(src|include|utils)/.*'"
    )
endif()

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

# --------------------
# Stress test client
# --------------------
add_executable(calc_client
    src/calcClient.cpp
    ${GENERATED_PROTO_SRCS}
)

# Client depends on proto_gen
add_dependencies(calc_client proto_gen)

# Stress client include directories
target_include_directories(calc_client
    PRIVATE
    ${GENERATED_DIR}
    SYSTEM PUBLIC
    ${Protobuf_INCLUDE_DIRS}
)

target_link_libraries(calc_client
    PRIVATE
    protobuf::libprotobuf
    gRPC::grpc++
    pthread
)