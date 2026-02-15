// handle_helpers.cpp — Implementation of handle utilities
#include "tools/handle_helpers.h"

#include <cmath>

#include "shapes/line.h"
#include "shapes/text_shape.h"

HandleType getHandleAt(QPointF point,
                       const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return HandleType::NONE;

  auto isNear = [&](double px, double py) {
    return std::abs(point.x() - px) <= HANDLE_TOLERANCE &&
           std::abs(point.y() - py) <= HANDLE_TOLERANCE;
  };

  // For lines: only check the two endpoints
  auto line = std::dynamic_pointer_cast<Line>(shape);
  if (line) {
    if (isNear(line->getX1(), line->getY1())) return HandleType::LINE_START;
    if (isNear(line->getX2(), line->getY2())) return HandleType::LINE_END;
    return HandleType::NONE;
  }

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

void drawSelectionHandles(QPainter& painter,
                          const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return;

  // For lines: draw only two endpoint circles
  auto line = std::dynamic_pointer_cast<Line>(shape);
  if (line) {
    // Dashed bounding box
    QRectF box = shape->boundingBox();
    QPen dashPen(Qt::cyan, 1, Qt::DashLine);
    painter.setPen(dashPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(box);

    // Endpoint handles as small filled circles
    painter.setPen(Qt::black);
    painter.setBrush(Qt::lightGray);
    double r = HANDLE_SIZE / 2.0;
    painter.drawEllipse(QPointF(line->getX1(), line->getY1()), r, r);
    painter.drawEllipse(QPointF(line->getX2(), line->getY2()), r, r);
    return;
  }

  auto text = std::dynamic_pointer_cast<TextShape>(shape);
  if (text) {
    QRectF box = shape->boundingBox();
    QColor hl(text->getFillColor().c_str());
    if (!hl.isValid() || text->getFillColor() == "transparent" ||
        text->getFillColor() == "none") {
      hl = QColor(0, 120, 212, 40);
    }
    QColor borderCol = hl;
    borderCol.setAlpha(std::min(255, borderCol.alpha() + 120));

    QPen dashPen(borderCol, 1, Qt::DashLine);
    painter.setPen(dashPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(box);
    return;
  }

  QRectF box = shape->boundingBox();

  QPen borderPen(Qt::cyan, 1, Qt::DashLine);
  painter.setPen(borderPen);
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(box);

  painter.setPen(Qt::black);
  painter.setBrush(Qt::lightGray);

  double x = box.x(), y = box.y();
  double w = box.width(), h = box.height();
  double hw = w / 2.0, hh = h / 2.0;

  QPointF handles[8] = {{x, y},          {x + hw, y},     {x + w, y},
                        {x, y + hh},     {x + w, y + hh}, {x, y + h},
                        {x + hw, y + h}, {x + w, y + h}};

  for (int i = 0; i < 8; i++) {
    painter.drawRect(QRectF(handles[i].x() - HANDLE_SIZE / 2,
                            handles[i].y() - HANDLE_SIZE / 2, HANDLE_SIZE,
                            HANDLE_SIZE));
  }
}

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
