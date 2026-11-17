cmake_minimum_required(VERSION 3.16)

project(prir_ex1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ex1
    src/main.cpp
    src/DataProcessor.cpp
    src/SequentialDataProcessor.cpp
    src/SimpleBlockProcessor.cpp
)

target_include_directories(ex1 PRIVATE include)

target_compile_options(ex1 PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
