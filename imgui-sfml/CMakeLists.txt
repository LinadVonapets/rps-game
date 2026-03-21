project(imgui-sfml VERSION 1.0)

file(GLOB IMGUI_SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${IMGUI_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} GL sfml-graphics sfml-system sfml-window)