cmake_minimum_required(VERSION 3.16)
project(calc LANGUAGES CXX)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Установка CMAKE_BUILD_TYPE
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    message(STATUS "Build type not specified, defaulting to Debug")
endif()

# Настройка флагов отладки
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Для clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_CLANG_TIDY "Enable clang-tidy checks" ON)

if(ENABLE_CLANG_TIDY)
    # Ищем разные версии clang-tidy
    find_program(CLANG_TIDY
        NAMES
        clang-tidy
        clang-tidy-18
        clang-tidy-17
        clang-tidy-16
        clang-tidy-15
        DOC "Path to clang-tidy executable"
    )

    if(CLANG_TIDY)
        message(STATUS "Found clang-tidy: ${CLANG_TIDY}")

        # Проверяем существование конфигурационного файла
        if(EXISTS "${CMAKE_SOURCE_DIR}/.clang-tidy")
            set(CMAKE_CXX_CLANG_TIDY
                "${CLANG_TIDY}"
                "--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
            )
            message(STATUS "Using clang-tidy config: ${CMAKE_SOURCE_DIR}/.clang-tidy")
        else()
            message(WARNING "No .clang-tidy config file found at ${CMAKE_SOURCE_DIR}/.clang-tidy")
            set(CMAKE_CXX_CLANG_TIDY
                "${CLANG_TIDY}"
                "-checks=modernize-*,readability-*,bugprone-*,performance-*,-modernize-use-trailing-return-type"
                "--warnings-as-errors=*"
            )
        endif()
    else()
        message(WARNING "clang-tidy not found. Install with: sudo apt-get install clang-tidy")
        set(ENABLE_CLANG_TIDY OFF CACHE BOOL "Enable clang-tidy" FORCE)
    endif()
endif()

# Подключаем библиотеку
add_subdirectory(lib)

# Создаем исполняемый файл
add_executable(calc main.cpp)

# Линкуем библиотеку
target_link_libraries(calc PRIVATE math)

# Установка
install(TARGETS calc
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT runtime
)

# Предупреждение о лицензии
message(STATUS "===========================================")
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Clang-tidy: ${ENABLE_CLANG_TIDY}")
message(STATUS "===========================================")