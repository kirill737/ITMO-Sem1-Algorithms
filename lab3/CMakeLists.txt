cmake_minimum_required(VERSION 3.10)
project(Array LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# --- Header-only библиотека Array ---
add_library(sort_lib INTERFACE)
target_include_directories(sort_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# --- Добавляем GoogleTest через FetchContent ---
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- Тестовый исполняемый файл ---
add_executable(find findThreshold.cpp)
add_executable(tests test_sort.cpp)
target_link_libraries(tests PRIVATE sort_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
