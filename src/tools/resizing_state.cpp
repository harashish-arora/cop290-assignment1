// resizing_state.cpp — Handles dragging a handle to resize a shape
#include "tools/resizing_state.h"

#include <algorithm>
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

ResizingState::ResizingState(HandleType handle, double left, double top,
                             double right, double bottom)
    : activeHandle(handle),
      anchorLeft(left),
      anchorTop(top),
      anchorRight(right),
      anchorBottom(bottom),
      oldBox(left, top, right - left, bottom - top) {}

void ResizingState::snapshotFreehandPoints(const std::vector<QPointF>& pts) {
  origFreehandPts = pts;
  // Compute bounding box of original points
  if (!pts.empty()) {
    double minX = pts[0].x(), maxX = minX;
    double minY = pts[0].y(), maxY = minY;
    for (auto& p : pts) {
      minX = std::min(minX, p.x());
      maxX = std::max(maxX, p.x());
      minY = std::min(minY, p.y());
      maxY = std::max(maxY, p.y());
    }
    origFreehandBox = QRectF(minX, minY, maxX - minX, maxY - minY);
  }
}

void ResizingState::handleMousePress(Canvas*, QMouseEvent*) {}

void ResizingState::handleMouseMove(Canvas* canvas, QMouseEvent* event) {
  if (!(event->buttons() & Qt::LeftButton)) return;

  auto& selected = canvas->getSelectedShape();
  if (!selected) return;

  QPointF pos = event->position();

  auto line = std::dynamic_pointer_cast<Line>(selected);
  if (line) {
    // Directly move the dragged endpoint
    if (activeHandle == HandleType::LINE_START) {
      line->setEndpoints(pos.x(), pos.y(), line->getX2(), line->getY2());
    } else if (activeHandle == HandleType::LINE_END) {
      line->setEndpoints(line->getX1(), line->getY1(), pos.x(), pos.y());
    }
    canvas->setLastMousePos(pos);
    canvas->update();
    return;
  }

  // Start from the saved anchors, only move the dragged edges
  double left = anchorLeft, top = anchorTop;
  double right = anchorRight, bottom = anchorBottom;

  switch (activeHandle) {
    case HandleType::TOP_LEFT:
      left = pos.x();
      top = pos.y();
      break;
    case HandleType::TOP:
      top = pos.y();
      break;
    case HandleType::TOP_RIGHT:
      right = pos.x();
      top = pos.y();
      break;
    case HandleType::LEFT:
      left = pos.x();
      break;
    case HandleType::RIGHT:
      right = pos.x();
      break;
    case HandleType::BOTTOM_LEFT:
      left = pos.x();
      bottom = pos.y();
      break;
    case HandleType::BOTTOM:
      bottom = pos.y();
      break;
    case HandleType::BOTTOM_RIGHT:
      right = pos.x();
      bottom = pos.y();
      break;
    default:
      break;
  }

  double newX = std::min(left, right);
  double newY = std::min(top, bottom);
  double newW = std::abs(right - left);
  double newH = std::abs(bottom - top);

  auto rect = std::dynamic_pointer_cast<Rectangle>(selected);
  if (rect) {
    QRectF box = selected->boundingBox();
    rect->moveBy(newX - box.x(), newY - box.y());
    rect->setGeometry(newW, newH);
  }

  auto rr = std::dynamic_pointer_cast<RoundedRectangle>(selected);
  if (rr) {
    QRectF box = selected->boundingBox();
    rr->moveBy(newX - box.x(), newY - box.y());
    rr->setGeometry(newW, newH);
  }

  auto circle = std::dynamic_pointer_cast<Circle>(selected);
  if (circle) {
    circle->setCenter(newX + newW / 2.0, newY + newH / 2.0);
    circle->setRadii(newW / 2.0, newH / 2.0);
  }

  auto hex = std::dynamic_pointer_cast<Hexagon>(selected);
  if (hex) {
    hex->setCenter(newX + newW / 2.0, newY + newH / 2.0);
    hex->setRadii(newW / 2.0, newH / 2.0);
  }

  auto fh = std::dynamic_pointer_cast<Freehand>(selected);
  if (fh && !origFreehandPts.empty() && origFreehandBox.width() > 0.5 &&
      origFreehandBox.height() > 0.5) {
    // Always transform from original snapshot — no oscillation
    auto& pts = const_cast<std::vector<QPointF>&>(fh->getPoints());
    for (size_t i = 0; i < pts.size() && i < origFreehandPts.size(); i++) {
      double tx = (origFreehandPts[i].x() - origFreehandBox.x()) /
                  origFreehandBox.width();
      double ty = (origFreehandPts[i].y() - origFreehandBox.y()) /
                  origFreehandBox.height();
      pts[i].setX(left + tx * (right - left));
      pts[i].setY(top + ty * (bottom - top));
    }
  }

  canvas->setLastMousePos(pos);
  canvas->update();
}

void ResizingState::handleMouseRelease(Canvas* canvas, QMouseEvent* event) {
  if (event->button() != Qt::LeftButton) return;
  auto& sel = canvas->getSelectedShape();
  if (sel) {
    QRectF newBox = sel->boundingBox();
    if (newBox != oldBox)
      canvas->pushCommand(std::make_unique<ResizeCommand>(sel, oldBox, newBox));
  }
  canvas->setState(std::make_unique<IdleState>());
  canvas->update();
}
