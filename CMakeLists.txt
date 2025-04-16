cmake_minimum_required(VERSION 3.24)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

project(BalancedTernary)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

add_executable(BalancedTernary
    src/trit.cpp    

    tests/trit.cpp
    tests/number.cpp
)

target_include_directories(BalancedTernary PRIVATE include)
target_link_libraries(BalancedTernary GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(BalancedTernary)