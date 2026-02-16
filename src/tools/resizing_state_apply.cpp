// resizing_state_apply.cpp
// implementation for applying resize handle drags to shape geometry updates

#include <algorithm>
#include <cmath>

#include "gui/canvas.h"
#include "shapes/circle.h"
#include "shapes/freehand.h"
#include "shapes/hexagon.h"
#include "shapes/rectangle.h"
#include "shapes/rounded_rectangle.h"
#include "tools/resizing_state.h"

// apply active resize handle movement to selected shape geometry
void ResizingState::applyResize(Canvas* canvas, QPointF pos) {
  auto& selected = canvas->getSelectedShape();
  double left = anchorLeft, top = anchorTop;
  double right = anchorRight, bottom = anchorBottom;

  // update dragged edges based on active handle
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

  // normalize to positive width and height
  double newX = std::min(left, right);
  double newY = std::min(top, bottom);
  double newW = std::abs(right - left);
  double newH = std::abs(bottom - top);

  // apply resized box to each supported shape type
  auto rect = std::dynamic_pointer_cast<Rectangle>(selected);
  if (rect) {
    QRectF box = selected->boundingBox();
    rect->moveBy(newX - box.x(), newY - box.y());
    rect->setGeometry(newW, newH);
  }

  // rounded rect is resized by moving top-left corner and setting new geometry
  auto rr = std::dynamic_pointer_cast<RoundedRectangle>(selected);
  if (rr) {
    QRectF box = selected->boundingBox();
    rr->moveBy(newX - box.x(), newY - box.y());
    rr->setGeometry(newW, newH);
  }

  // circle is resized by moving center and setting new radii
  auto circle = std::dynamic_pointer_cast<Circle>(selected);
  if (circle) {
    circle->setCenter(newX + newW / 2.0, newY + newH / 2.0);
    circle->setRadii(newW / 2.0, newH / 2.0);
  }

  // keep hex orientation and only update center and radii
  auto hex = std::dynamic_pointer_cast<Hexagon>(selected);
  if (hex) {
    hex->setCenter(newX + newW / 2.0, newY + newH / 2.0);
    hex->setRadii(newW / 2.0, newH / 2.0);
  }

  // freehand is remapped by relative point coordinates
  auto fh = std::dynamic_pointer_cast<Freehand>(selected);
  if (fh && !origFreehandPts.empty() && origFreehandBox.width() > 0.5 &&
      origFreehandBox.height() > 0.5) {
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

  // store last mouse position for state bookkeeping and repaint
  canvas->setLastMousePos(pos);
  canvas->update();
}
