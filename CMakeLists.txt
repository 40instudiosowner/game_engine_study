cmake_minimum_required(VERSION 3.5)
project(MegaManECS
    VERSION 3.0
    LANGUAGES CXX)

file(COPY
    assets/
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

file(COPY
    config.json
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(dependencies)
add_subdirectory(src)