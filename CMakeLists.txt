cmake_minimum_required(VERSION 3.24)

project(BalancedTernary)

set(CMAKE_CXX_STANDARD 20)

add_executable(BalancedTernary
    src/trit.cpp    
    src/main.cpp
)

target_include_directories(BalancedTernary PRIVATE include)