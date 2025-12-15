add_library(math SHARED math.cpp) # или SHARED для динамической
target_include_directories(math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
