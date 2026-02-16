// hexagon.h
// Regular hexagon shape (fits inside a bounding box)
#pragma once
#include "shapes/graphics_object.h"

class Hexagon : public GraphicsObject {
 private:
  // center and radii (half-width, half-height) defining the bounding box
  double cx, cy, rx, ry;
  bool pointyTop = false;       // false = flat-top, true = pointy-top
  QPolygonF hexPoints() const;  // compute polygon points in local coords

 public:
  // construct with center (cx,cy) and half-sizes rx/ry.
  Hexagon(double cx, double cy, double rx, double ry);

  // polymorphic overrides for drawing, hit-testing, bounding box and cloning
  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;
  QRectF boundingBox() const override;

  // geometry mutators and helpers
  void setCenter(double x, double y);
  void setRadii(double rx, double ry);
  void moveBy(double dx, double dy) override;
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;

  // control pointy vs flat top orientation
  bool isPointyTop() const;
  void setPointyTop(bool pt);
};
