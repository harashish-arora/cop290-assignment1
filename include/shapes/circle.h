// circle.h
// Circle shape class definition
#pragma once
#include "shapes/graphics_object.h"

class Circle : public GraphicsObject {
 private:
  // center (cx, cy) and radii (rx, ry) for ellipse support
  double cx, cy;
  double rx, ry;

 public:
  // constructors for circle and ellipse
  Circle(double x, double y, double r);  // for circle on initial construction
  Circle(double x, double y, double rx, double ry);  // constructor on editing

  // overrides for drawing, SVG conversion, hit testing, and bounding box
  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;

  QRectF boundingBox() const override;

  // Setters
  void setRadius(double r);
  void setRadii(double rx, double ry);
  void setCenter(double x, double y);
  void moveBy(double dx, double dy) override;

  // cloning via deep copy
  std::shared_ptr<GraphicsObject> clone() const override;

  // restore from the bounding box
  void setFromBoundingBox(const QRectF& box) override;
};
