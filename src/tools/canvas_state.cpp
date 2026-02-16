// canvas_state.cpp
// abstract base class for canvas interaction states like idle, drawing and
// dragging

#include "tools/canvas_state.h"

// virtual destructor for polymorphic base class
CanvasState::~CanvasState() = default;

// default key handler does nothing subclasses override when needed
void CanvasState::handleKeyPress(Canvas*, QKeyEvent*) {}
