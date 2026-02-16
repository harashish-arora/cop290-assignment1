// rounded_rectangle.h
// Rectangle with rounded corners
#pragma once
#include "shapes/graphics_object.h"

class RoundedRectangle : public GraphicsObject {
 private:
  double x, y;    // top-left
  double rx, ry;  // corner radii

 public:
  // Create rounded rectangle at (x,y) with width/height and optional radii
  RoundedRectangle(double x, double y, double w, double h, double rx = 10,
                   double ry = 10);

  // drawing, SVG conversion, hit testing, and bounding box overrides
  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double mouseX, double mouseY) const override;
  QRectF boundingBox() const override;

  // update position and geometry
  void setGeometry(double nw, double nh);
  void setCornerRadius(double r);
  double getCornerRadius() const;
  void moveBy(double dx, double dy) override;

  // deep copy and restoring from box
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;
};
