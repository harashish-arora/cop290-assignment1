// moving_state.h — State for dragging to move a selected shape
#pragma once
#include "tools/canvas_state.h"

class MovingState : public CanvasState {
 public:
  void handleMousePress(Canvas* canvas, QMouseEvent* event) override;
  void handleMouseMove(Canvas* canvas, QMouseEvent* event) override;
  void handleMouseRelease(Canvas* canvas, QMouseEvent* event) override;

 private:
  double totalDx = 0, totalDy = 0;
};
