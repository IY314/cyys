cmake_minimum_required(VERSION 3.23)

set(CYYS_SOURCES "main.cpp" "pos.cpp" "tokenizer.cpp")

add_executable(cyys ${CYYS_SOURCES})

target_include_directories(cyys PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
