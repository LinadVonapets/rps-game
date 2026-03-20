cmake_minimum_required(VERSION 4.2.0)
project(rps_life_game VERSION 0.1-dev)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(SFML 2.5 
              COMPONENTS 
              system window graphics network audio REQUIRED)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

file(GLOB rps_life_game_src "src/*.cpp")

add_executable(${PROJECT_NAME} ${rps_life_game_src})

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-system sfml-window sfml-network) 

add_dependencies(${PROJECT_NAME} copy_assets)