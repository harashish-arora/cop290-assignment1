// hexagon_geo.cpp
// hexagon shape geometry and hit-testing

#include "shapes/hexagon.h"

// compute hexagon points in local coordinates based on center and radii
bool Hexagon::contains(double x, double y) const {
  return hexPoints().containsPoint(QPointF(x, y), Qt::OddEvenFill);
}

// bounding box from center and radii
QRectF Hexagon::boundingBox() const {
  return QRectF(cx - rx, cy - ry, 2 * rx, 2 * ry);
}

// center setter
void Hexagon::setCenter(double x, double y) {
  cx = x;
  cy = y;
}

// radii setter with cached width height update
void Hexagon::setRadii(double newRx, double newRy) {
  rx = newRx;
  ry = newRy;
  width = 2 * rx;
  height = 2 * ry;
}

// translate center
void Hexagon::moveBy(double dx, double dy) {
  cx += dx;
  cy += dy;
}

// clone with orientation and style
std::shared_ptr<GraphicsObject> Hexagon::clone() const {
  auto copy = std::make_shared<Hexagon>(cx, cy, rx, ry);
  copy->setPointyTop(pointyTop);
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

// restore geometry from axis aligned box
void Hexagon::setFromBoundingBox(const QRectF& box) {
  cx = box.center().x();
  cy = box.center().y();
  rx = box.width() / 2.0;
  ry = box.height() / 2.0;
  width = 2 * rx;
  height = 2 * ry;
}
