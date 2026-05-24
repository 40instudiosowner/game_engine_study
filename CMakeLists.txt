cmake_minimum_required(VERSION 3.5)
project(SpaceInvaders
    VERSION 1.3
    LANGUAGES CXX)

file(COPY
    assets/fonts/futura.ttf
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/fonts)


file(COPY
    assets/images/player.png
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/images)

file(COPY
    config.json
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(dependencies)
add_subdirectory(src) 