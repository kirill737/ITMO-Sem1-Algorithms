cmake_minimum_required(VERSION 3.10)
project(Array LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# --- Header-only библиотека Array ---
add_library(array_lib INTERFACE)
target_include_directories(array_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# --- GoogleTest через FetchContent ---
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- Тестовый исполняемый файл ---
add_executable(tests test_array.cpp)
target_link_libraries(tests PRIVATE array_lib gtest gtest_main)

# --- Автоматическое обнаружение тестов ---
include(GoogleTest)
gtest_discover_tests(tests)
