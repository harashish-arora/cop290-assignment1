// shape_mode.h
// enum used to pick the canvas interaction mode (create, select, edit)
#pragma once

// controls how input on the Canvas is interpreted.
// current mode stored on the Canvas and used by
// CanvasState to change mouse and keyboard behaviour
enum class ShapeMode {
  SELECT,
  RECTANGLE,
  CIRCLE,
  HEXAGON,
  LINE,
  ROUNDED_RECT,
  FREEHAND,
  TEXT
};
