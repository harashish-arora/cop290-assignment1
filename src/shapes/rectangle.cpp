// rectangle.cpp
#include "shapes/rectangle.h"

#include <algorithm>

// constructor

Rectangle::Rectangle(double x, double y, double w, double h)
    : x(x), y(y), width(w), height(h) {}

void Rectangle::draw(QPainter& painter) const {
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
  painter.drawRect(QRectF(x, y, width, height));
}

bool Rectangle::contains(double mouseX, double mouseY) const {
  // find the boundaries since w and h can be negative
  double left = std::min(x, x + width);
  double right = std::max(x, x + width);
  double top = std::min(y, y + height);
  double bottom = std::max(y, y + height);

  // check if the mouse is inside these boundaries
  bool insideX = (mouseX >= left) && (mouseX <= right);
  bool insideY = (mouseY >= top) && (mouseY <= bottom);

  return insideX && insideY;
}
