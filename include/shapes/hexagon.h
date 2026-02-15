// hexagon.h — Regular hexagon shape (flat-top, fits inside bounding box)
#pragma once
#include "shapes/graphics_object.h"

class Hexagon : public GraphicsObject {
 public:
  // cx, cy = center; rx, ry = half-width, half-height of bounding box
  Hexagon(double cx, double cy, double rx, double ry);

  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;
  QRectF boundingBox() const override;

  void setCenter(double x, double y);
  void setRadii(double rx, double ry);
  void moveBy(double dx, double dy) override;
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;

  bool isPointyTop() const;
  void setPointyTop(bool pt);

 private:
  double cx, cy, rx, ry;
  bool pointyTop = false;  // false = flat-top, true = pointy-top
  QPolygonF hexPoints() const;
};
