// circle cpp
// circle shape class definition

#include "shapes/circle.h"

#include <cmath>
#include <string>

// constructor for regular circle stores equal radii
Circle::Circle(double x, double y, double r) : cx(x), cy(y), rx(r), ry(r) {
  this->width = 2 * rx;
  this->height = 2 * ry;
}

// constructor for ellipse with independent radii
Circle::Circle(double x, double y, double rx, double ry)
    : cx(x), cy(y), rx(rx), ry(ry) {
  this->width = 2 * rx;
  this->height = 2 * ry;
}

// draw shape using fill and stroke properties
void Circle::draw(QPainter& painter) const {
  QPen pen(QColor(strokeColor.c_str()));
  pen.setWidthF(strokeWidth);
  painter.setPen(pen);

  if (fillColor == "none") {
    painter.setBrush(Qt::NoBrush);
  } else {
    painter.setBrush(QBrush(QColor(fillColor.c_str())));
  }

  painter.drawEllipse(QPointF(cx, cy), rx, ry);
}

// convert object state to svg ellipse tag
std::string Circle::toSVG() const {
  return "<ellipse cx=\"" + std::to_string(cx) + "\" " + "cy=\"" +
         std::to_string(cy) + "\" " + "rx=\"" + std::to_string(rx) + "\" " +
         "ry=\"" + std::to_string(ry) + "\" " +
         svgColorAttr("stroke", strokeColor) + " " + "stroke-width=\"" +
         std::to_string(strokeWidth) + "\" " + svgColorAttr("fill", fillColor) +
         " />";
}

// hit test using normalized ellipse equation
bool Circle::contains(double x, double y) const {
  if (rx <= 0 || ry <= 0) return false;
  double dx = (x - cx) / rx;
  double dy = (y - cy) / ry;
  return (dx * dx + dy * dy) <= 1.0;
}

// axis aligned bounding box for selection and handles
QRectF Circle::boundingBox() const {
  return QRectF(cx - rx, cy - ry, 2 * rx, 2 * ry);
}

// update both radii for circle mode
void Circle::setRadius(double r) {
  rx = r;
  ry = r;
  width = 2 * r;
  height = 2 * r;
}

// update radii independently for ellipse behavior
void Circle::setRadii(double newRx, double newRy) {
  rx = newRx;
  ry = newRy;
  width = 2 * rx;
  height = 2 * ry;
}

// move center directly
void Circle::setCenter(double x, double y) {
  cx = x;
  cy = y;
}

// translate by delta
void Circle::moveBy(double dx, double dy) {
  cx += dx;
  cy += dy;
}

// clone with style fields copied for clipboard and commands
std::shared_ptr<GraphicsObject> Circle::clone() const {
  auto copy = std::make_shared<Circle>(cx, cy, rx, ry);
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

// rebuild geometry from bounding box for resize undo redo
void Circle::setFromBoundingBox(const QRectF& box) {
  cx = box.center().x();
  cy = box.center().y();
  rx = box.width() / 2.0;
  ry = box.height() / 2.0;
  width = 2 * rx;
  height = 2 * ry;
}
