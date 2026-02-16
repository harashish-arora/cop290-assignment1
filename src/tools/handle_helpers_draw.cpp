// handle_helpers_draw.cpp — Drawing selection handles
#include "shapes/line.h"
#include "shapes/text_shape.h"
#include "tools/handle_helpers.h"

void drawSelectionHandles(QPainter& painter,
                          const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return;

  auto line = std::dynamic_pointer_cast<Line>(shape);
  if (line) {
    QRectF box = shape->boundingBox();
    QPen dashPen(Qt::cyan, 1, Qt::DashLine);
    painter.setPen(dashPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(box);
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
