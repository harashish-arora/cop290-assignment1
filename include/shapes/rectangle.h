// rectangle.h
#pragma once
#include "shapes/graphics_object.h"

class Rectangle : public GraphicsObject {
 private:
  double x, y, width, height;

 public:
  Rectangle(double x, double y, double w, double h);

  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;
};
