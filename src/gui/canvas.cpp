// canvas.cpp — Core: constructor, paint, mouse/key event dispatch
#include "gui/canvas.h"

#include <QPainter>
#include <QPainterPath>
#include <cmath>

#include "shapes/freehand.h"
#include "shapes/line.h"
#include "tools/handle_helpers.h"
#include "tools/idle_state.h"

Canvas::Canvas(QWidget* parent) : QWidget(parent) {
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  currentState = std::make_unique<IdleState>();
  textEditing = false;
}

void Canvas::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.fillRect(rect(), Qt::white);
  painter.setRenderHint(QPainter::Antialiasing);
  for (const auto& shape : shapes) shape->draw(painter);

  if (previewShape) {
    QPen dash(Qt::black, 1, Qt::DashLine);
    painter.setPen(dash);
    painter.setBrush(Qt::NoBrush);
    QRectF box = previewShape->boundingBox();

    if (getMode() == ShapeMode::CIRCLE) {
      painter.drawEllipse(box);
    } else if (getMode() == ShapeMode::HEXAGON) {
      QPolygonF poly;
      double pcx = box.center().x(), pcy = box.center().y();
      double prx = box.width() / 2.0, pry = box.height() / 2.0;
      for (int i = 0; i < 6; i++) {
        double a = 3.14159265358979 / 180.0 * (60.0 * i);
        poly << QPointF(pcx + prx * std::cos(a), pcy + pry * std::sin(a));
      }
      painter.drawPolygon(poly);
    } else if (getMode() == ShapeMode::LINE) {
      auto* line = dynamic_cast<Line*>(previewShape.get());
      if (line)
        painter.drawLine(QPointF(line->getX1(), line->getY1()),
                         QPointF(line->getX2(), line->getY2()));
    } else if (getMode() == ShapeMode::FREEHAND) {
      auto* fh = dynamic_cast<Freehand*>(previewShape.get());
      if (fh) {
        const auto& pts = fh->getPoints();
        if (pts.size() >= 2) {
          QPainterPath path;
          path.moveTo(pts[0]);
          for (size_t i = 1; i < pts.size(); i++) path.lineTo(pts[i]);
          painter.drawPath(path);
        }
      }
    } else if (getMode() == ShapeMode::ROUNDED_RECT) {
      painter.drawRoundedRect(box, 10, 10);
    } else {
      painter.drawRect(box);
    }
  }

  if (selectedShape) {
    drawSelectionHandles(painter, selectedShape);
  }
}

void Canvas::mousePressEvent(QMouseEvent* e) {
  if (textEditing) finalizeTextEditing();
  currentState->handleMousePress(this, e);
}
void Canvas::mouseMoveEvent(QMouseEvent* e) {
  currentState->handleMouseMove(this, e);
}
void Canvas::mouseReleaseEvent(QMouseEvent* e) {
  currentState->handleMouseRelease(this, e);
}
