cmake_minimum_required(VERSION 3.16)
project(GraphicsEditor)
include_directories(include)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ProjectInkscape 
    main.cpp 
    src/shapes/graphics_object.cpp
    src/shapes/rectangle.cpp
    src/gui/canvas.cpp
    src/gui/main_window.cpp
    src/gui/properties_panel.cpp
    src/gui/tool_bar.cpp
    src/shapes/circle.cpp
    src/shapes/hexagon.cpp
    src/shapes/line.cpp
    src/shapes/rounded_rectangle.cpp
    src/shapes/freehand.cpp
    src/shapes/text_shape.cpp
    src/tools/handle_helpers.cpp
    src/tools/idle_state.cpp
    src/tools/creating_state.cpp
    src/tools/moving_state.cpp
    src/tools/resizing_state.cpp
    src/tools/command.cpp
    src/parse/svg_parser.cpp
    include/gui/canvas.h
    include/gui/main_window.h
    include/gui/properties_panel.h
    include/gui/tool_bar.h
)

target_link_libraries(ProjectInkscape PRIVATE Qt6::Widgets)
