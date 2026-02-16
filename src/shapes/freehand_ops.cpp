// freehand_ops.cpp — Freehand move, clone, scaling
#include "shapes/freehand.h"

void Freehand::moveBy(double dx, double dy) {
  for (auto& pt : points) {
    pt.setX(pt.x() + dx);
    pt.setY(pt.y() + dy);
  }
}

std::shared_ptr<GraphicsObject> Freehand::clone() const {
  auto copy = std::make_shared<Freehand>();
  copy->points = points;
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

void Freehand::setFromBoundingBox(const QRectF& box) {
  scaleToBox(box.left(), box.top(), box.right(), box.bottom());
}

void Freehand::scaleToBox(double left, double top, double right,
                          double bottom) {
  QRectF oldBox = boundingBox();
  if (oldBox.width() < 1 || oldBox.height() < 1) return;
  for (auto& pt : points) {
    double tx = (pt.x() - oldBox.x()) / oldBox.width();
    double ty = (pt.y() - oldBox.y()) / oldBox.height();
    pt.setX(left + tx * (right - left));
    pt.setY(top + ty * (bottom - top));
  }
}
