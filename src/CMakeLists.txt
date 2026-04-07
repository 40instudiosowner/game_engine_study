set(TARGET_NAME main)
set(SOURCES main.cpp
    Window.cpp
    Rectangle.cpp
    Text.cpp 
    ConfigReader.cpp
    UI.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in DEBUG mode with DEBUG=1")
    add_compile_definitions(DEBUG=1)
    if (MSVC)
        # For MSVC, use /Zi for debug info
        # set(CMAKE_CXX_FLAGS_DEBUG "/Zi")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # For GCC/Clang, use -g and common warnings
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    endif()
endif()


add_executable(${TARGET_NAME} 
    ${SOURCES})

#target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
target_link_libraries(${TARGET_NAME} PUBLIC ImGui-SFML::ImGui-SFML)
target_link_libraries(${TARGET_NAME} PUBLIC nlohmann_json::nlohmann_json)

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(${TARGET_NAME} PUBLIC ${X11_LIBRARIES})
    target_include_directories(${TARGET_NAME} PUBLIC ${X11_INCLUDE_DIR})
endif()