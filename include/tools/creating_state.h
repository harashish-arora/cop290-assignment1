// creating_state.h — State for dragging to create a new shape
#pragma once
#include "tools/canvas_state.h"

class CreatingState : public CanvasState {
 public:
  void handleMousePress(Canvas* canvas, QMouseEvent* event) override;
  void handleMouseMove(Canvas* canvas, QMouseEvent* event) override;
  void handleMouseRelease(Canvas* canvas, QMouseEvent* event) override;
};
