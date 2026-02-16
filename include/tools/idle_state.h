// idle_state.h
// fsm state for when nothing is being dragged
#pragma once
#include "tools/canvas_state.h"

class IdleState : public CanvasState {
 public:
  void handleMousePress(Canvas* canvas, QMouseEvent* event) override;
  void handleMouseMove(Canvas* canvas, QMouseEvent* event) override;
  void handleMouseRelease(Canvas* canvas, QMouseEvent* event) override;
  void handleKeyPress(Canvas* canvas, QKeyEvent* event) override;
};
