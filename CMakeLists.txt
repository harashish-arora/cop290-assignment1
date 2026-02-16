cmake_minimum_required(VERSION 3.16)
project(GraphicsEditor)
include_directories(include)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ProjectInkscape 
    main.cpp 
    src/shapes/graphics_object.cpp
    src/shapes/rectangle.cpp
    src/gui/canvas.cpp
    src/gui/canvas_paint.cpp
    src/gui/canvas_events.cpp
    src/gui/canvas_clipboard.cpp
    src/gui/canvas_history.cpp
    src/gui/canvas_text.cpp
    src/gui/canvas_file.cpp
    src/gui/unsaved_changes_dialog.cpp
    src/gui/main_window.cpp
    src/gui/main_window_menus.cpp
    src/gui/properties_panel.cpp
    src/gui/properties_panel_helpers.cpp
    src/gui/properties_panel_grid.cpp
    src/gui/properties_panel_connect.cpp
    src/gui/properties_panel_state.cpp
    src/gui/properties_panel_refresh.cpp
    src/gui/properties_panel_dialogs.cpp
    src/gui/tool_bar.cpp
    src/gui/tool_bar_icons.cpp
    src/gui/app_style.cpp
    src/shapes/circle.cpp
    src/shapes/hexagon.cpp
    src/shapes/hexagon_geo.cpp
    src/shapes/line.cpp
    src/shapes/rounded_rectangle.cpp
    src/shapes/freehand.cpp
    src/shapes/freehand_ops.cpp
    src/shapes/text_shape.cpp
    src/shapes/text_shape_ops.cpp
    src/tools/handle_helpers.cpp
    src/tools/handle_helpers_draw.cpp
    src/tools/idle_state.cpp
    src/tools/idle_state_create.cpp
    src/tools/creating_state.cpp
    src/tools/moving_state.cpp
    src/tools/resizing_state.cpp
    src/tools/resizing_state_apply.cpp
    src/tools/command.cpp
    src/tools/shape_property_command.cpp
    src/parse/svg_parser.cpp
    src/parse/svg_parser_utils.cpp
    src/parse/svg_parser_shapes.cpp
    include/gui/canvas.h
    include/gui/main_window.h
    include/gui/properties_panel.h
    include/gui/tool_bar.h
    include/gui/properties_panel_helpers.h
    include/gui/shape_mode.h
    include/gui/app_style.h
    include/gui/unsaved_changes_dialog.h
    include/tools/shape_property_command.h
    include/tools/shape_style_defaults.h
    include/parse/svg_parser_internal.h
)

target_link_libraries(ProjectInkscape PRIVATE Qt6::Widgets)
