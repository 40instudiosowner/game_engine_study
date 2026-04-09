cmake_minimum_required(VERSION 3.5)
project(Slider
    VERSION 1.3
    LANGUAGES CXX)

file(COPY
    assets/fonts/futura.ttf
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/fonts)

file(COPY
    assets/images/BluRayLogo.png
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/images)
file(COPY
    assets/images/CDLogo.png
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/images)
file(COPY
    assets/images/DVDLogo.png
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/images)

file(COPY
    config.json
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(dependencies)
add_subdirectory(src) 