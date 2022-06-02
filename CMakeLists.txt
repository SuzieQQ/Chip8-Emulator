cmake_minimum_required (VERSION 3.2)
project(Chip8-Emulator)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

file(GLOB SRC "src/*.cpp" "src/*.hpp" "src/*.h")

add_executable(Chip8-Emulator ${SRC})



# imgui setup
add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui_sdl/imgui_sdl.cpp

)

# include headers
target_include_directories(imgui
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui_sdl/
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
)


set(SDL2_BUILDING_LIBRARY ON)
find_package(SDL2 REQUIRED)
find_package(fmt  REQUIRED)
include_directories(CHIP8-EMULATOR PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Chip8-Emulator PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(Chip8-Emulator PRIVATE fmt::fmt )
target_link_libraries(Chip8-Emulator PRIVATE imgui)


