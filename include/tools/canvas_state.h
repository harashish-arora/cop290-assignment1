// canvas_state.h
// Abstract base for all canvas interaction states
#pragma once
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <memory>

class Canvas;  // forward declaration

// abstract base class representing current state of canvas input handling
// different states implement different mouse and keyboard behavior for
// creating shapes, selecting, moving etc
class CanvasState {
 public:
  virtual ~CanvasState();

  virtual void handleMousePress(Canvas* canvas, QMouseEvent* event) = 0;
  virtual void handleMouseMove(Canvas* canvas, QMouseEvent* event) = 0;
  virtual void handleMouseRelease(Canvas* canvas, QMouseEvent* event) = 0;
  virtual void handleKeyPress(Canvas*, QKeyEvent*);
};
