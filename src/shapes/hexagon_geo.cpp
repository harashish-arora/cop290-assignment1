// hexagon_geo.cpp — Hexagon geometry: contains, bbox, transform, clone
#include "shapes/hexagon.h"

bool Hexagon::contains(double x, double y) const {
  return hexPoints().containsPoint(QPointF(x, y), Qt::OddEvenFill);
}

QRectF Hexagon::boundingBox() const {
  return QRectF(cx - rx, cy - ry, 2 * rx, 2 * ry);
}

void Hexagon::setCenter(double x, double y) {
  cx = x;
  cy = y;
}

void Hexagon::setRadii(double newRx, double newRy) {
  rx = newRx;
  ry = newRy;
  width = 2 * rx;
  height = 2 * ry;
}

void Hexagon::moveBy(double dx, double dy) {
  cx += dx;
  cy += dy;
}

std::shared_ptr<GraphicsObject> Hexagon::clone() const {
  auto copy = std::make_shared<Hexagon>(cx, cy, rx, ry);
  copy->setPointyTop(pointyTop);
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

void Hexagon::setFromBoundingBox(const QRectF& box) {
  cx = box.center().x();
  cy = box.center().y();
  rx = box.width() / 2.0;
  ry = box.height() / 2.0;
  width = 2 * rx;
  height = 2 * ry;
}
