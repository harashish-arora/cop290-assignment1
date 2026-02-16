# ProjectInkscape

This application provides an intuitive interface for creating and editing 2D vector graphics with a focus on simplicity and performance.

## Features

### Drawing Tools

- **Rectangle**: Create standard rectangles with customizable dimensions
- **Rounded Rectangle**: Rectangles with adjustable corner radius
- **Circle**: Perfect circles and ellipses
- **Hexagon**: Regular and irregular hexagons
- **Line**: Straight line segments with configurable stroke
- **Freehand**: Free-form drawing with smooth curves
- **Text**: Text objects with font customization

### Editing Capabilities

- **Selection Tool**: Select, move, and resize objects
- **Interactive Resizing**: Drag handles to resize shapes proportionally
- **Properties Panel**: Real-time property editing for selected shapes
- **Undo/Redo**: Full undo/redo support with command pattern
- **Clipboard Operations**: Copy, cut, and paste shapes
- **Delete**: Remove selected objects

### File Operations

- **SVG Import/Export**: Full SVG file format support
- **Save/Save As**: Persistent storage of drawings
- **Unsaved Changes Detection**: Automatic tracking of modifications
- **New File**: Start fresh with a clean canvas

### Styling Options

- Customizable fill colors
- Adjustable stroke colors and widths
- Visual feedback during object creation and manipulation

## Prerequisites

- **CMake**: Version 3.16 or higher
- **C++ Compiler**: Supporting C++17 standard
- **Qt6**: Qt 6 with Widgets module

## Building

The project uses CMake for configuration and building. In the root directory:

```bash
cmake -B build && cmake --build build
```

This will:

1. Configure the project in the `build` directory
2. Compile all source files
3. Link the Qt6 libraries
4. Generate the executable `ProjectInkscape`

### Build Options

For a release build with optimizations:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

For a debug build with debugging symbols:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Running

After building, run the application:

```bash
./build/ProjectInkscape
```

## Project Structure

```txt
.
├── CMakeLists.txt          # Build configuration
├── main.cpp                # Application entry point
├── include/                # Header files
│   ├── gui/                # GUI components
│   │   ├── canvas.h        # Main drawing canvas
│   │   ├── main_window.h  # Application window
│   │   ├── properties_panel.h  # Shape properties editor
│   │   └── tool_bar.h      # Drawing tools toolbar
│   ├── shapes/             # Shape classes
│   │   ├── graphics_object.h   # Base class for all shapes
│   │   ├── circle.h
│   │   ├── rectangle.h
│   │   ├── hexagon.h
│   │   ├── line.h
│   │   ├── rounded_rectangle.h
│   │   ├── freehand.h
│   │   └── text_shape.h
│   ├── tools/              # Interaction states and commands
│   │   ├── canvas_state.h  # State pattern for canvas modes
│   │   ├── command.h       # Command pattern for undo/redo
│   │   └── ...
│   └── parse/              # File I/O
│       └── svg_parser.h    # SVG import/export
└── src/                    # Implementation files
    ├── gui/                # GUI implementations
    ├── shapes/             # Shape implementations
    ├── tools/              # Tools and commands
    └── parse/              # File parsing
```

## Architecture

### Design Patterns

- **State Pattern**: Canvas interaction modes (selecting, drawing, resizing) are implemented as separate state classes
- **Command Pattern**: All modifications support undo/redo through command objects
- **Observer Pattern**: Qt's signal-slot mechanism for UI updates
- **Polymorphism**: All shapes inherit from `GraphicsObject` base class

### Key Components

- **Canvas**: Main drawing area that handles rendering and user input
- **MainWindow**: Top-level window with menu bar and layout management
- **PropertiesPanel**: Side panel for editing selected shape properties
- **ToolBar**: Toolbar for switching between drawing tools
- **GraphicsObject**: Abstract base class defining the interface for all drawable shapes
- **SVGParser**: Handles loading and saving drawings in SVG format

## Design Decisions

### Finite State Machine (FSM) for Canvas Interaction

The canvas uses an FSM to manage different interaction modes. The `CanvasState` abstract base class defines the interface, with concrete states:

- **IdleState**: Default state for selection and tool switching
- **CreatingState**: Active while drawing a new shape
- **MovingState**: Active when dragging a selected shape
- **ResizingState**: Active when resizing via corner handles

Each state implements distinct `handleMousePress()`, `handleMouseMove()`, and `handleMouseRelease()` behaviors. The canvas delegates all input events to its current state, ensuring clean separation of interaction logic and eliminating complex conditional branches.

### Command Pattern for Undo/Redo

All canvas modifications (create, delete, move, resize, property changes) are encapsulated as command objects implementing the `Command` interface with `execute()` and `undo()` methods. Commands are stored in undo/redo stacks, enabling full history navigation without coupling the UI to shape implementation details.

### Modular Code Organization

The codebase enforces a strict limit of **under 100 lines per file** to maintain readability and single-responsibility principles. Complex classes are split across multiple files (e.g., `canvas.cpp`, `canvas_paint.cpp`, `canvas_file.cpp`), with each file handling a focused subset of functionality.

### SVG as Native Format

SVG was chosen as the file format for its:
- **Openness**: Text-based, human-readable XML
- **Interoperability**: Widely supported across tools and browsers
- **Scalability**: Vector format preserves quality at any resolution
- **Extensibility**: Easy to extend with custom attributes for application-specific data

### Shape Hierarchy with Polymorphism

All shapes inherit from the `GraphicsObject` abstract base class, which defines a uniform interface for rendering (`paint()`), hit testing (`contains()`), bounds calculation (`getBounds()`), and serialization. This allows the canvas to treat all shapes uniformly while each shape implements its specific geometry and rendering logic.

### Qt Signal-Slot Architecture

The application leverages Qt's signal-slot mechanism for loose coupling between components. For example, toolbar selections emit signals that the canvas connects to, and property panel changes trigger signals that update the canvas state. This implements the Observer pattern without manual dependency management.

## Usage

1. **Launch** the application
2. **Select a tool** from the toolbar (Rectangle, Circle, etc.)
3. **Click and drag** on the canvas to create shapes
4. **Click** on existing shapes to select them
5. **Modify properties** using the properties panel on the right
6. **Save** your work via File → Save or File → Save As
7. **Load** existing SVG files via File → Open

### Keyboard Shortcuts

- **Ctrl+Z / Cmd+Z**: Undo
- **Ctrl+Y / Cmd+Shift+Z**: Redo
- **Ctrl+C / Cmd+C**: Copy
- **Ctrl+X / Cmd+X**: Cut
- **Ctrl+V / Cmd+V**: Paste
- **Delete / Backspace**: Delete selected shape
- **Ctrl+N / Cmd+N**: New file
- **Ctrl+O / Cmd+O**: Open file
- **Ctrl+S / Cmd+S**: Save
- **Ctrl+Shift+S / Cmd+Shift+S**: Save As

## Technical Details

- **Language**: C++17
- **GUI Framework**: Qt6 (Widgets module)
- **Build System**: CMake 3.16+
- **File Format**: SVG (Scalable Vector Graphics)
- **Code Organization**: Modular architecture with under 100 lines per file

## Development

The codebase follows modern C++ practices with:

- Smart pointers for automatic memory management
- Const correctness
- Separation of concerns with modular file structure
- Clean code principles with focused, single-responsibility classes

## License

This project is part of COP290 (Design Practices in Computer Science) coursework.
