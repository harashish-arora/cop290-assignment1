// canvas_state.h — Abstract base for all canvas interaction states
#pragma once
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <memory>

class Canvas;  // Forward declaration

class CanvasState {
 public:
  virtual ~CanvasState();

  virtual void handleMousePress(Canvas* canvas, QMouseEvent* event) = 0;
  virtual void handleMouseMove(Canvas* canvas, QMouseEvent* event) = 0;
  virtual void handleMouseRelease(Canvas* canvas, QMouseEvent* event) = 0;
  virtual void handleKeyPress(Canvas*, QKeyEvent*);
};
