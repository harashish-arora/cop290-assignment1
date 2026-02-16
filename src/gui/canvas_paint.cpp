// canvas paint cpp
// implementation for canvas paint

#include "gui/canvas.h"

#include <QPainter>
#include <QPainterPath>
#include <cmath>

#include "shapes/freehand.h"
#include "shapes/hexagon.h"
#include "shapes/line.h"
#include "shapes/text_shape.h"
#include "tools/handle_helpers.h"

// paint flow
// draw committed shapes first then draw preview while creating then selection handles
void Canvas::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.fillRect(rect(), Qt::white);
  painter.setRenderHint(QPainter::Antialiasing);
  for (const auto& shape : shapes) {
    if (textEditing) {
      auto txt = std::dynamic_pointer_cast<TextShape>(shape);
      if (txt && selectedShape && shape == selectedShape) continue;
    }
    shape->draw(painter);
  }

  if (previewShape) {
    // preview uses dashed outline so user can see shape being created
    QPen dash(Qt::black, 1, Qt::DashLine);
    painter.setPen(dash);
    painter.setBrush(Qt::NoBrush);
    QRectF box = previewShape->boundingBox();
    if (getMode() == ShapeMode::CIRCLE) {
      painter.drawEllipse(box);
    } else if (getMode() == ShapeMode::HEXAGON) {
      auto* hex = dynamic_cast<Hexagon*>(previewShape.get());
      QPolygonF poly;
      double cx = box.center().x(), cy = box.center().y();
      double rx = box.width() / 2.0, ry = box.height() / 2.0;
      double offset = (hex && hex->isPointyTop()) ? (3.14159265358979 / 6.0) : 0.0;
      for (int i = 0; i < 6; i++) {
        double a = 3.14159265358979 / 180.0 * (60.0 * i) + offset;
        poly << QPointF(cx + rx * std::cos(a), cy + ry * std::sin(a));
      }
      painter.drawPolygon(poly);
    } else if (getMode() == ShapeMode::LINE) {
      auto* line = dynamic_cast<Line*>(previewShape.get());
      if (line)
        painter.drawLine(QPointF(line->getX1(), line->getY1()),
                         QPointF(line->getX2(), line->getY2()));
    } else if (getMode() == ShapeMode::FREEHAND) {
      auto* fh = dynamic_cast<Freehand*>(previewShape.get());
      if (fh && fh->getPoints().size() >= 2) {
        const auto& pts = fh->getPoints();
        QPainterPath path;
        path.moveTo(pts[0]);
        for (size_t i = 1; i < pts.size(); i++) path.lineTo(pts[i]);
        painter.drawPath(path);
      }
    } else if (getMode() == ShapeMode::ROUNDED_RECT) {
      painter.drawRoundedRect(box, 10, 10);
    } else {
      painter.drawRect(box);
    }
  }

  if (selectedShape && !textEditing) drawSelectionHandles(painter, selectedShape);
}
