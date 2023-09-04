
cmake_minimum_required(VERSION 3.22.1)

file(GLOB_RECURSE CALCULATOR_SOURCES "*.cpp")

add_library(
        MyCalculator
        SHARED
        ${CALCULATOR_SOURCES}
)

target_include_directories(
        MyCalculator
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)