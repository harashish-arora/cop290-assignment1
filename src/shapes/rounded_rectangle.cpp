// rounded rectangle cpp
// rectangle with rounded corners

#include "shapes/rounded_rectangle.h"

#include <algorithm>
#include <cmath>

// constructor stores position size and corner radii
RoundedRectangle::RoundedRectangle(double x, double y, double w, double h,
                                   double rx, double ry)
    : x(x), y(y), rx(rx), ry(ry) {
  width = w;
  height = h;
}

// normalized box supports negative drag directions
QRectF RoundedRectangle::boundingBox() const {
  return QRectF(x, y, width, height).normalized();
}

// draw rounded rectangle with fill and stroke
void RoundedRectangle::draw(QPainter& painter) const {
  QPen pen(QColor(strokeColor.c_str()));
  pen.setWidthF(strokeWidth);
  painter.setPen(pen);

  if (fillColor == "none") {
    painter.setBrush(Qt::NoBrush);
  } else {
    painter.setBrush(QBrush(QColor(fillColor.c_str())));
  }

  QRectF r = boundingBox();
  painter.drawRoundedRect(r, rx, ry);
}

// hit test uses normalized bounding box
bool RoundedRectangle::contains(double mouseX, double mouseY) const {
  return boundingBox().contains(mouseX, mouseY);
}

// serialize shape to svg rect with rx ry attributes
std::string RoundedRectangle::toSVG() const {
  QRectF r = boundingBox();
  return "<rect x=\"" + std::to_string(r.x()) + "\" y=\"" +
         std::to_string(r.y()) + "\" width=\"" + std::to_string(r.width()) +
         "\" height=\"" + std::to_string(r.height()) + "\" rx=\"" +
         std::to_string(rx) + "\" ry=\"" + std::to_string(ry) + "\" " +
         svgColorAttr("stroke", strokeColor) + " stroke-width=\"" +
         std::to_string(strokeWidth) + "\" " + svgColorAttr("fill", fillColor) +
         " />";
}

// update width and height from drag values
void RoundedRectangle::setGeometry(double nw, double nh) {
  width = nw;
  height = nh;
}

// keep uniform corner radius in x and y
void RoundedRectangle::setCornerRadius(double r) {
  rx = r;
  ry = r;
}

// return active corner radius for panel sync
double RoundedRectangle::getCornerRadius() const { return rx; }

// translate top left point
void RoundedRectangle::moveBy(double dx, double dy) {
  x += dx;
  y += dy;
}

// clone with style values copied
std::shared_ptr<GraphicsObject> RoundedRectangle::clone() const {
  auto copy = std::make_shared<RoundedRectangle>(x, y, width, height, rx, ry);
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

// restore geometry from undo redo bounding box
void RoundedRectangle::setFromBoundingBox(const QRectF& box) {
  x = box.x();
  y = box.y();
  width = box.width();
  height = box.height();
}
