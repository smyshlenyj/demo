add_library(math STATIC math.cpp)
target_include_directories(math PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
