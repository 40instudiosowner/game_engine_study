set(TARGET_NAME main)
set(SOURCES main.cpp
    GameEngine.cpp
    AssetManager.cpp
    InputManager.cpp
    # Scenes
    Scenes/MenuScene.cpp
    Scenes/GameScene.cpp
    # Systems
    Systems/AnimationSystem.cpp
    Systems/CameraSystem.cpp
    Systems/PlayerControlSystem.cpp
    Systems/PlayerAnimationSystem.cpp
    Systems/GravitySystem.cpp
    Systems/BrickSystem.cpp
    Systems/MovementSystem.cpp
    Systems/CollisionSystem.cpp
    Systems/TileCollisionSystem.cpp
    Systems/ShooterSystem.cpp
    Systems/RenderSystem.cpp
    )

add_executable(${TARGET_NAME} 
    ${SOURCES})

# Copy assets on every build so config changes take effect immediately
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in DEBUG mode with DEBUG=1")
    add_compile_definitions(DEBUG=1)
    if (MSVC)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    endif()
endif()

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
target_link_libraries(${TARGET_NAME} PUBLIC ImGui-SFML::ImGui-SFML)
target_link_libraries(${TARGET_NAME} PUBLIC nlohmann_json::nlohmann_json)

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(${TARGET_NAME} PUBLIC ${X11_LIBRARIES})
    target_include_directories(${TARGET_NAME} PUBLIC ${X11_INCLUDE_DIR})
endif()