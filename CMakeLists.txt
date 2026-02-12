cmake_minimum_required(VERSION 3.16)
project(GraphicsEditor)
include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(main 
    main.cpp 
    src/shapes/graphics_object.cpp
    src/shapes/rectangle.cpp
    src/gui/canvas.cpp
)

target_link_libraries(main PRIVATE Qt6::Widgets)
