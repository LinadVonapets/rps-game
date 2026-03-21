cmake_minimum_required(VERSION 4.2.0)
project(rps_life_game LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS off)

include(FetchContent)

FetchContent_Declare(
   SFML
   GIT_REPOSITORY https://github.com/SFML/SFML.git
   GIT_TAG 3.0.2
   GIT_SHALLOW ON
   EXCLUDE_FROM_ALL
   SYSTEM
)

FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG v1.91.9
	GIT_SHALLOW ON
	EXCLUDE_FROM_ALL
	SYSTEM
)

FetchContent_Declare(
	imgui-sfml
	GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
	GIT_TAG v3.0
	GIT_SHALLOW ON 
	EXCLUDE_FROM_ALL
	SYSTEM
)

FetchContent_MakeAvailable(SFML)
FetchContent_MakeAvailable(imgui)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(imgui-sfml)


add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/Entity.cpp
	src/EntityGroupSystem.cpp
	src/Core.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    SFML::Graphics
    SFML::Audio
    SFML::System
    SFML::Window
    SFML::Network
	ImGui-SFML::ImGui-SFML
) 

add_dependencies(${PROJECT_NAME} copy_assets)
