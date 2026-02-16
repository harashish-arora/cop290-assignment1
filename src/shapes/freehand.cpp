// freehand cpp
// freehand polyline shape sequence of points

#include "shapes/freehand.h"

#include <QPainterPath>
#include <algorithm>
#include <cmath>

// default freehand style is no fill black stroke width one
Freehand::Freehand() {
  fillColor = "none";
  strokeColor = "black";
  strokeWidth = 1;
}

// append one sampled point to the polyline
void Freehand::addPoint(double x, double y) { points.emplace_back(x, y); }

// return immutable point list for drawing and resize logic
const std::vector<QPointF>& Freehand::getPoints() const { return points; }

// draw polyline path when at least two points exist
void Freehand::draw(QPainter& painter) const {
  if (points.size() < 2) return;
  QPen pen(QColor(strokeColor.c_str()));
  pen.setWidthF(strokeWidth);
  pen.setCapStyle(Qt::RoundCap);
  pen.setJoinStyle(Qt::RoundJoin);
  painter.setPen(pen);
  painter.setBrush(Qt::NoBrush);

  QPainterPath path;
  path.moveTo(points[0]);
  for (size_t i = 1; i < points.size(); i++) {
    path.lineTo(points[i]);
  }
  painter.drawPath(path);
}

// compute tight bounding box around all points
QRectF Freehand::boundingBox() const {
  if (points.empty()) return QRectF(0, 0, 0, 0);
  double minX = points[0].x(), maxX = minX;
  double minY = points[0].y(), maxY = minY;
  for (auto& pt : points) {
    minX = std::min(minX, pt.x());
    maxX = std::max(maxX, pt.x());
    minY = std::min(minY, pt.y());
    maxY = std::max(maxY, pt.y());
  }
  return QRectF(minX, minY, maxX - minX, maxY - minY);
}

// hit test by checking distance to every segment
bool Freehand::contains(double x, double y) const {
  // check if x,y is close to any segment of the polyline
  const double tolerance = 6.0;
  for (size_t i = 1; i < points.size(); i++) {
    double x1 = points[i - 1].x(), y1 = points[i - 1].y();
    double x2 = points[i].x(), y2 = points[i].y();
    double dx = x2 - x1, dy = y2 - y1;
    double lenSq = dx * dx + dy * dy;
    double t = (lenSq > 0) ? ((x - x1) * dx + (y - y1) * dy) / lenSq : 0;
    t = std::max(0.0, std::min(1.0, t));
    double px = x1 + t * dx, py = y1 + t * dy;
    double dist = std::sqrt((x - px) * (x - px) + (y - py) * (y - py));
    if (dist <= tolerance) return true;
  }
  return false;
}

// serialize polyline with freehand marker attribute
std::string Freehand::toSVG() const {
  if (points.empty()) return "";
  std::string pts;
  for (size_t i = 0; i < points.size(); i++) {
    if (i > 0) pts += " ";
    pts += std::to_string(points[i].x()) + "," + std::to_string(points[i].y());
  }
  return "<polyline data-shape=\"freehand\" points=\"" + pts + "\" " +
         svgColorAttr("stroke", strokeColor) + " stroke-width=\"" +
         std::to_string(strokeWidth) + "\" " + "fill=\"none\" " +
         "stroke-linecap=\"round\" stroke-linejoin=\"round\" />";
}
