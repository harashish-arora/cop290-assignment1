// rounded_rectangle.cpp — Rectangle with rounded corners
#include "shapes/rounded_rectangle.h"

#include <algorithm>
#include <cmath>

RoundedRectangle::RoundedRectangle(double x, double y, double w, double h,
                                   double rx, double ry)
    : x(x), y(y), rx(rx), ry(ry) {
  width = w;
  height = h;
}

QRectF RoundedRectangle::boundingBox() const {
  return QRectF(x, y, width, height).normalized();
}

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

bool RoundedRectangle::contains(double mouseX, double mouseY) const {
  return boundingBox().contains(mouseX, mouseY);
}

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

void RoundedRectangle::setGeometry(double nw, double nh) {
  width = nw;
  height = nh;
}

void RoundedRectangle::setCornerRadius(double r) {
  rx = r;
  ry = r;
}

double RoundedRectangle::getCornerRadius() const { return rx; }

void RoundedRectangle::moveBy(double dx, double dy) {
  x += dx;
  y += dy;
}

std::shared_ptr<GraphicsObject> RoundedRectangle::clone() const {
  auto copy = std::make_shared<RoundedRectangle>(x, y, width, height, rx, ry);
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

void RoundedRectangle::setFromBoundingBox(const QRectF& box) {
  x = box.x();
  y = box.y();
  width = box.width();
  height = box.height();
}
