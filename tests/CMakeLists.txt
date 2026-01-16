add_executable(calc_tests
    testSanity.cpp

    # ../src/calculator.cpp
    # ../src/loggerWrapper.cpp
)

target_link_libraries(calc_tests
    PRIVATE
    math
    nlohmann_json::nlohmann_json
    spdlog
    gtest_main
)

# Не прогоняем clang-tidy по тестам и gtest
set_target_properties(calc_tests PROPERTIES CXX_CLANG_TIDY "")

target_include_directories(calc_tests
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

include(GoogleTest)
gtest_discover_tests(calc_tests)
