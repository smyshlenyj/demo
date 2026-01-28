# --------------------
# Tests CMakeLists.txt
# --------------------
add_executable(calc_tests
    testSanity.cpp

    # ../src/calculator.cpp
    # ../src/loggerWrapper.cpp
)

# Linking libraries
target_link_libraries(calc_tests
    PRIVATE
    math
    nlohmann_json::nlohmann_json
    spdlog
    gtest_main
)

# Turn off clang-tidy for tests and third-party
set_target_properties(calc_tests PROPERTIES CXX_CLANG_TIDY "")

# include
target_include_directories(calc_tests
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

# GoogleTest discovery
include(GoogleTest)
gtest_discover_tests(calc_tests)

# --------------------
# Run tests after build
# --------------------
add_custom_command(TARGET calc_tests
    POST_BUILD
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
)

# --------------------
# Valgrind test
# --------------------
add_test(NAME calc_valgrind
    COMMAND valgrind --leak-check=full --show-leak-kinds=all ./calc_tests
)

set_tests_properties(calc_valgrind PROPERTIES
    TIMEOUT 120
)
