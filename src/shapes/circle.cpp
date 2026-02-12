// circle.cpp
#include "shapes/circle.h"

// constructor

Circle::Circle(double x, double y, double r) : cx(x), cy(y), radius(r) {}

// draw

void Circle::draw(QPainter& painter) const {
  // the outline
  QPen pen(QColor(strokeColor.c_str()));
  pen.setWidthF(strokeWidth);
  painter.setPen(pen);

  // setting the fill
  if (fillColor == "none") {
    painter.setBrush(Qt::NoBrush);
  } else {
    painter.setBrush(QBrush(QColor(fillColor.c_str())));
  }

  // drawing the pixels
  // use an ellipse, but draw using the radius as same for both x and y axes
  painter.drawEllipse(QPointF(cx, cy), radius, radius);
}

// to SVG as string

std::string Circle::toSVG() const {
  return "<circle cx=\"" + std::to_string(cx) + "\" " + "cy=\"" +
         std::to_string(cy) + "\" " + "r=\"" + std::to_string(radius) + "\" " +
         "stroke=\"" + strokeColor + "\" " + "stroke-width=\"" +
         std::to_string(strokeWidth) + "\" " + "fill=\"" + fillColor + "\" />";
}

// contains check using pythagoroean theorem

bool Circle::contains(double x, double y) const {
  double delta_x = x - cx;
  double delta_y = y - cy;

  return (delta_x * delta_x + delta_y * delta_y) <= (radius * radius);
}
