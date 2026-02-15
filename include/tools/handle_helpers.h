// handle_helpers.h — Shared handle utilities (hit-test, drawing, cursor)
#pragma once
#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QWidget>
#include <memory>

#include "shapes/graphics_object.h"

const int HANDLE_SIZE = 8;
const double HANDLE_TOLERANCE = HANDLE_SIZE + 4;

enum class HandleType {
  NONE,
  TOP_LEFT,
  TOP,
  TOP_RIGHT,
  LEFT,
  RIGHT,
  BOTTOM_LEFT,
  BOTTOM,
  BOTTOM_RIGHT,
  LINE_START,
  LINE_END
};

// Check if a point is near a resize handle on the shape's bounding box
HandleType getHandleAt(QPointF point,
                       const std::shared_ptr<GraphicsObject>& shape);

// Draw the 8 resize handles around a shape
void drawSelectionHandles(QPainter& painter,
                          const std::shared_ptr<GraphicsObject>& shape);

// Set the cursor based on which handle is being hovered
void updateCursorForHandle(QWidget* widget, HandleType handle);
