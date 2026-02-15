// line.h — Line shape (two-endpoint segment)
#pragma once
#include "shapes/graphics_object.h"

class Line : public GraphicsObject {
 public:
  Line(double x1, double y1, double x2, double y2);

  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;
  QRectF boundingBox() const override;

  void setEndpoints(double x1, double y1, double x2, double y2);
  double getX1() const;
  double getY1() const;
  double getX2() const;
  double getY2() const;

  void moveBy(double dx, double dy) override;
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;

 private:
  double x1, y1, x2, y2;
};
