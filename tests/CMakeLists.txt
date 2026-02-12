# --------------------
# Tests CMakeLists.txt
# --------------------
set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
set(PROTO_SRCS
    ${GENERATED_DIR}/calc.pb.cc
    ${GENERATED_DIR}/calc.grpc.pb.cc
)

# Warnings turn off for protobuf generated code
set_source_files_properties(${PROTO_SRCS} PROPERTIES COMPILE_FLAGS "-w")

# --------------------
# Tests
# --------------------
add_executable(calc_tests
    testSanity.cpp
    calculatorGrpcTest.cpp
)

# Add generated files to tests
add_dependencies(calc_tests proto_gen)

# Linking
target_link_libraries(calc_tests
    PRIVATE
    math
    spdlog
    gtest_main
    protobuf::libprotobuf
    gRPC::grpc++
    pthread
)

# Include
target_include_directories(calc_tests
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/utils
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    SYSTEM PRIVATE ${GENERATED_DIR}
)

# --------------------
# Turn on TSan for tests
# --------------------

# target_compile_options(calc_tests PRIVATE -fsanitize=thread -fno-omit-frame-pointer -g)
# target_link_options(calc_tests PRIVATE -fsanitize=thread)

# GoogleTest discovery
include(GoogleTest)
gtest_discover_tests(calc_tests)

# Run tests after build
add_custom_command(TARGET calc_tests
    POST_BUILD
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
)

# Valgrind test
add_test(NAME calc_valgrind
    COMMAND valgrind --leak-check=full --show-leak-kinds=all ./calc_tests
)
set_tests_properties(calc_valgrind PROPERTIES TIMEOUT 120)
