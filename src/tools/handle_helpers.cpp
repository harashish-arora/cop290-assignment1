// handle_helpers.cpp
// shape selection handle hit testing and cursor updates

#include "tools/handle_helpers.h"

#include <cmath>

#include "shapes/line.h"
#include "shapes/text_shape.h"

// hit test a point against the selection handles of a shape
HandleType getHandleAt(QPointF point,
                       const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return HandleType::NONE;

  auto isNear = [&](double px, double py) {
    return std::abs(point.x() - px) <= HANDLE_TOLERANCE &&
           std::abs(point.y() - py) <= HANDLE_TOLERANCE;
  };

  // line exposes only two endpoint handles
  auto line = std::dynamic_pointer_cast<Line>(shape);
  if (line) {
    if (isNear(line->getX1(), line->getY1())) return HandleType::LINE_START;
    if (isNear(line->getX2(), line->getY2())) return HandleType::LINE_END;
    return HandleType::NONE;
  }

  // text is not resized by handles
  // there are no handles for text
  auto text = std::dynamic_pointer_cast<TextShape>(shape);
  if (text) return HandleType::NONE;

  QRectF box = shape->boundingBox();
  double x = box.x(), y = box.y();
  double w = box.width(), h = box.height();
  double hw = w / 2.0, hh = h / 2.0;

  if (isNear(x, y)) return HandleType::TOP_LEFT;
  if (isNear(x + hw, y)) return HandleType::TOP;
  if (isNear(x + w, y)) return HandleType::TOP_RIGHT;
  if (isNear(x, y + hh)) return HandleType::LEFT;
  if (isNear(x + w, y + hh)) return HandleType::RIGHT;
  if (isNear(x, y + h)) return HandleType::BOTTOM_LEFT;
  if (isNear(x + hw, y + h)) return HandleType::BOTTOM;
  if (isNear(x + w, y + h)) return HandleType::BOTTOM_RIGHT;

  return HandleType::NONE;
}

// set cursor icon according to hovered handle type depending on mode
void updateCursorForHandle(QWidget* widget, HandleType handle) {
  switch (handle) {
    case HandleType::TOP_LEFT:
    case HandleType::BOTTOM_RIGHT:
      widget->setCursor(Qt::SizeFDiagCursor);
      break;
    case HandleType::TOP_RIGHT:
    case HandleType::BOTTOM_LEFT:
      widget->setCursor(Qt::SizeBDiagCursor);
      break;
    case HandleType::TOP:
    case HandleType::BOTTOM:
      widget->setCursor(Qt::SizeVerCursor);
      break;
    case HandleType::LEFT:
    case HandleType::RIGHT:
      widget->setCursor(Qt::SizeHorCursor);
      break;
    case HandleType::LINE_START:
    case HandleType::LINE_END:
      widget->setCursor(Qt::CrossCursor);
      break;
    default:
      widget->setCursor(Qt::ArrowCursor);
      break;
  }
}
