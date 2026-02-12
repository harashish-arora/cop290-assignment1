// circle.h
#pragma once
#include "shapes/graphics_object.h"

class Circle : public GraphicsObject {
 private:
  double cx, cy, radius;

 public:
  Circle(double cx, double cy, double radius);

  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;
};
