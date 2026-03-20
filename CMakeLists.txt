cmake_minimum_required(VERSION 4.2.0)
project(rps_life_game LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS off)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

file(GLOB rps_life_game_src "src/*.cpp")

add_executable(${PROJECT_NAME} ${rps_life_game_src})

target_link_libraries(
    ${PROJECT_NAME}
    SFML::Graphics 
    SFML::Audio
    SFML::System
    SFML::Window
    SFML::Network
) 

add_dependencies(${PROJECT_NAME} copy_assets)