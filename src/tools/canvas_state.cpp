// canvas state cpp
// abstract base for all canvas interaction states

#include "tools/canvas_state.h"

// virtual destructor for polymorphic base class
CanvasState::~CanvasState() = default;

// default key handler does nothing subclasses override when needed
void CanvasState::handleKeyPress(Canvas*, QKeyEvent*) {}
