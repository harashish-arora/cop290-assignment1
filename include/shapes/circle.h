#pragma once
#include "shapes/graphics_object.h"

class Circle : public GraphicsObject {
 private:
  double cx, cy;
  double rx, ry;

 public:
  Circle(double x, double y, double r);
  Circle(double x, double y, double rx, double ry);

  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;

  QRectF boundingBox() const override;

  // Setters
  void setRadius(double r);
  void setRadii(double rx, double ry);
  void setCenter(double x, double y);
  void moveBy(double dx, double dy) override;

  // Clone (deep copy)
  std::shared_ptr<GraphicsObject> clone() const override;

  // Restore from bounding box
  void setFromBoundingBox(const QRectF& box) override;
};
