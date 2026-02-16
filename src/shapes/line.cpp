// line.cpp
// line segment shape defined by two endpoints

#include "shapes/line.h"

#include <algorithm>
#include <cmath>
#include <string>

// construct line from two endpoints and cache width and height
Line::Line(double x1, double y1, double x2, double y2)
    : x1(x1), y1(y1), x2(x2), y2(y2) {
  width = std::abs(x2 - x1);
  height = std::abs(y2 - y1);
}

// draw line using stroke properties, no fill
void Line::draw(QPainter& painter) const {
  QPen pen(QColor(strokeColor.c_str()));
  pen.setWidthF(strokeWidth);
  painter.setPen(pen);
  painter.setBrush(Qt::NoBrush);
  painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
}

// convert line to svg line tag
std::string Line::toSVG() const {
  return "<line x1=\"" + std::to_string(x1) + "\" y1=\"" + std::to_string(y1) +
         "\" x2=\"" + std::to_string(x2) + "\" y2=\"" + std::to_string(y2) +
         "\" " + svgColorAttr("stroke", strokeColor) + " stroke-width=\"" +
         std::to_string(strokeWidth) + "\" />";
}

// hit test by measuring distance to line segment
bool Line::contains(double mx, double my) const {
  // distance from point mx, my to the line segment x1,y1 x2,y2
  // checks closeness to endpoints and projection onto segment for hit testing
  double dx = x2 - x1;
  double dy = y2 - y1;
  double lenSq = dx * dx + dy * dy;
  if (lenSq < 1e-6) return std::hypot(mx - x1, my - y1) < 5.0;

  double t = ((mx - x1) * dx + (my - y1) * dy) / lenSq;
  t = std::clamp(t, 0.0, 1.0);
  double px = x1 + t * dx;
  double py = y1 + t * dy;
  return std::hypot(mx - px, my - py) < 5.0;
}

// return tight bounding box around endpoints
QRectF Line::boundingBox() const {
  double minX = std::min(x1, x2);
  double minY = std::min(y1, y2);
  return QRectF(minX, minY, std::abs(x2 - x1), std::abs(y2 - y1));
}

// update endpoints and cached width height
void Line::setEndpoints(double nx1, double ny1, double nx2, double ny2) {
  x1 = nx1;
  y1 = ny1;
  x2 = nx2;
  y2 = ny2;
  width = std::abs(x2 - x1);
  height = std::abs(y2 - y1);
}

// endpoint getters used by handles and preview drawing
double Line::getX1() const { return x1; }
double Line::getY1() const { return y1; }
double Line::getX2() const { return x2; }
double Line::getY2() const { return y2; }

// translate both endpoints
void Line::moveBy(double dx, double dy) {
  x1 += dx;
  y1 += dy;
  x2 += dx;
  y2 += dy;
}

// clone line with styles for clipboard and commands
std::shared_ptr<GraphicsObject> Line::clone() const {
  auto copy = std::make_shared<Line>(x1, y1, x2, y2);
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

// map endpoints proportionally to the new bounding box
// used by resize command replay
void Line::setFromBoundingBox(const QRectF& box) {
  // map endpoints from current bounding box to new box
  QRectF cur = boundingBox();
  double sx = (cur.width() > 1e-6) ? box.width() / cur.width() : 1.0;
  double sy = (cur.height() > 1e-6) ? box.height() / cur.height() : 1.0;
  x1 = box.x() + (x1 - cur.x()) * sx;
  y1 = box.y() + (y1 - cur.y()) * sy;
  x2 = box.x() + (x2 - cur.x()) * sx;
  y2 = box.y() + (y2 - cur.y()) * sy;
  width = std::abs(x2 - x1);
  height = std::abs(y2 - y1);
}
