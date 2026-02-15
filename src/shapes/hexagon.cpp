// hexagon.cpp
#include "shapes/hexagon.h"

#include <cmath>
#include <string>

static constexpr double PI = 3.14159265358979323846;

Hexagon::Hexagon(double cx, double cy, double rx, double ry)
    : cx(cx), cy(cy), rx(rx), ry(ry) {
  width = 2 * rx;
  height = 2 * ry;
}

QPolygonF Hexagon::hexPoints() const {
  QPolygonF poly;
  double offset = pointyTop ? (PI / 6.0) : 0.0;  // 30° offset for pointy-top
  for (int i = 0; i < 6; i++) {
    double angle = PI / 180.0 * (60.0 * i) + offset;
    poly << QPointF(cx + rx * std::cos(angle), cy + ry * std::sin(angle));
  }
  return poly;
}

bool Hexagon::isPointyTop() const { return pointyTop; }
void Hexagon::setPointyTop(bool pt) { pointyTop = pt; }

void Hexagon::draw(QPainter& painter) const {
  QPen pen(QColor(strokeColor.c_str()));
  pen.setWidthF(strokeWidth);
  painter.setPen(pen);
  if (fillColor == "none") {
    painter.setBrush(Qt::NoBrush);
  } else {
    painter.setBrush(QBrush(QColor(fillColor.c_str())));
  }
  painter.drawPolygon(hexPoints());
}

std::string Hexagon::toSVG() const {
  // SVG <polygon points="x1,y1 x2,y2 ...">
  QPolygonF pts = hexPoints();
  std::string pointsStr;
  for (int i = 0; i < pts.size(); i++) {
    if (i > 0) pointsStr += " ";
    pointsStr += std::to_string(pts[i].x()) + "," + std::to_string(pts[i].y());
  }
  return "<polygon data-shape=\"hexagon\" "
         "data-cx=\"" +
         std::to_string(cx) +
         "\" "
         "data-cy=\"" +
         std::to_string(cy) +
         "\" "
         "data-rx=\"" +
         std::to_string(rx) +
         "\" "
         "data-ry=\"" +
         std::to_string(ry) +
         "\" "
         "data-orientation=\"" +
         std::string(pointyTop ? "pointy" : "flat") +
         "\" "
         "points=\"" +
         pointsStr + "\" " + svgColorAttr("stroke", strokeColor) + " " +
         "stroke-width=\"" + std::to_string(strokeWidth) + "\" " +
         svgColorAttr("fill", fillColor) + " />";
}

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
