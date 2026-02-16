// hexagon cpp
// regular hexagon shape fits inside a bounding box

#include "shapes/hexagon.h"

#include <cmath>
#include <string>

static constexpr double PI = 3.14159265358979323846;

// constructor stores center and radii values
Hexagon::Hexagon(double cx, double cy, double rx, double ry)
    : cx(cx), cy(cy), rx(rx), ry(ry) {
  width = 2 * rx;
  height = 2 * ry;
}

// compute polygon points with optional pointy top angular offset
QPolygonF Hexagon::hexPoints() const {
  QPolygonF poly;
  double offset = pointyTop ? (PI / 6.0) : 0.0;  // 30 offset for pointy top
  for (int i = 0; i < 6; i++) {
    double angle = PI / 180.0 * (60.0 * i) + offset;
    poly << QPointF(cx + rx * std::cos(angle), cy + ry * std::sin(angle));
  }
  return poly;
}

// orientation accessors used by properties panel parser and preview
bool Hexagon::isPointyTop() const { return pointyTop; }
void Hexagon::setPointyTop(bool pt) { pointyTop = pt; }

// draw hexagon fill and stroke
void Hexagon::draw(QPainter& painter) const {
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
  painter.drawPolygon(hexPoints());
}

// serialize shape as svg polygon with custom hexagon metadata
std::string Hexagon::toSVG() const {
  // svg polygon points x1,y1 x2,y2
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
