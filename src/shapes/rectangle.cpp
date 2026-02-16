// rectangle.cpp
#include "shapes/rectangle.h"

#include <algorithm>

// Constructor
Rectangle::Rectangle(double x, double y, double w, double h) : x(x), y(y) {
  // We must set these in the body because they belong to the Parent class
  this->width = w;
  this->height = h;
}

// bounding box
QRectF Rectangle::boundingBox() const { return QRectF(x, y, width, height); }

// Draw
void Rectangle::draw(QPainter& painter) const {
  QPen pen(QColor(strokeColor.c_str()));
  pen.setWidthF(strokeWidth);
  pen.setJoinStyle(Qt::MiterJoin);
  pen.setMiterLimit(10.0);
  painter.setPen(pen);

  if (fillColor == "none") {
    painter.setBrush(Qt::NoBrush);
  } else {
    painter.setBrush(QBrush(QColor(fillColor.c_str())));
  }

  painter.drawRect(QRectF(x, y, width, height));
}

// Contains Check
bool Rectangle::contains(double mouseX, double mouseY) const {
  return boundingBox().contains(mouseX, mouseY);
}

// SVG Output
std::string Rectangle::toSVG() const {
  return "<rect x=\"" + std::to_string(x) + "\" " + "y=\"" + std::to_string(y) +
         "\" " + "width=\"" + std::to_string(width) + "\" " + "height=\"" +
         std::to_string(height) + "\" " + svgColorAttr("stroke", strokeColor) +
         " " + "stroke-width=\"" + std::to_string(strokeWidth) + "\" " +
         svgColorAttr("fill", fillColor) + " />";
}

// Resize (Update the parent fields)
void Rectangle::setGeometry(double nw, double nh) {
  width = nw;
  height = nh;
}

// Move
void Rectangle::moveBy(double dx, double dy) {
  x += dx;
  y += dy;
}

// Clone — returns an independent deep copy
std::shared_ptr<GraphicsObject> Rectangle::clone() const {
  auto copy = std::make_shared<Rectangle>(x, y, width, height);
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

void Rectangle::setFromBoundingBox(const QRectF& box) {
  x = box.x();
  y = box.y();
  width = box.width();
  height = box.height();
}
