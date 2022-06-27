cmake_minimum_required(VERSION 3.23)

project(cyys LANGUAGES CXX VERSION 0.01)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
