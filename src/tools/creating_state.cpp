// creating_state.cpp — Handles drag-to-create a new shape
#include "tools/creating_state.h"

#include <cmath>

#include "gui/canvas.h"
#include "shapes/circle.h"
#include "shapes/freehand.h"
#include "shapes/hexagon.h"
#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "shapes/rounded_rectangle.h"
#include "tools/command.h"
#include "tools/idle_state.h"

void CreatingState::handleMousePress(Canvas*, QMouseEvent*) {
  // Already creating — nothing to do
}

void CreatingState::handleMouseMove(Canvas* canvas, QMouseEvent* event) {
  if (!(event->buttons() & Qt::LeftButton)) return;

  auto& preview = canvas->getPreviewShape();
  if (!preview) return;

  QPointF current = event->position();
  QPointF start = canvas->getStartPoint();

  if (canvas->getMode() == ShapeMode::FREEHAND) {
    auto fh = std::dynamic_pointer_cast<Freehand>(preview);
    if (fh) fh->addPoint(current.x(), current.y());
  } else if (canvas->getMode() == ShapeMode::CIRCLE) {
    double r = std::sqrt(std::pow(current.x() - start.x(), 2) +
                         std::pow(current.y() - start.y(), 2));
    auto circle = std::dynamic_pointer_cast<Circle>(preview);
    if (circle) circle->setRadius(r);
  } else if (canvas->getMode() == ShapeMode::HEXAGON) {
    double dx = std::abs(current.x() - start.x());
    double dy = std::abs(current.y() - start.y());
    auto hex = std::dynamic_pointer_cast<Hexagon>(preview);
    if (hex) hex->setRadii(dx, dy);
  } else if (canvas->getMode() == ShapeMode::LINE) {
    auto line = std::dynamic_pointer_cast<Line>(preview);
    if (line)
      line->setEndpoints(start.x(), start.y(), current.x(), current.y());
  } else if (canvas->getMode() == ShapeMode::ROUNDED_RECT) {
    double w = current.x() - start.x();
    double h = current.y() - start.y();
    auto rr = std::dynamic_pointer_cast<RoundedRectangle>(preview);
    if (rr) rr->setGeometry(w, h);
  } else {
    double w = current.x() - start.x();
    double h = current.y() - start.y();
    auto rect = std::dynamic_pointer_cast<Rectangle>(preview);
    if (rect) rect->setGeometry(w, h);
  }

  canvas->update();
}

void CreatingState::handleMouseRelease(Canvas* canvas, QMouseEvent* event) {
  if (event->button() != Qt::LeftButton) return;

  auto& preview = canvas->getPreviewShape();
  if (preview) {
    QRectF box = preview->boundingBox();
    if (std::abs(box.width()) > 2 || std::abs(box.height()) > 2) {
      canvas->getShapes().push_back(preview);
      canvas->setSelectedShape(preview);
      canvas->pushCommand(std::make_unique<AddShapeCommand>(preview));
    }
    canvas->setPreviewShape(nullptr);
  }

  canvas->setState(std::make_unique<IdleState>());
  canvas->update();
}
