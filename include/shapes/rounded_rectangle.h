// rounded_rectangle.h — Rectangle with rounded corners
#pragma once
#include "shapes/graphics_object.h"

class RoundedRectangle : public GraphicsObject {
 private:
  double x, y;
  double rx, ry;  // corner radii

 public:
  RoundedRectangle(double x, double y, double w, double h, double rx = 10,
                   double ry = 10);

  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double mouseX, double mouseY) const override;
  QRectF boundingBox() const override;

  void setGeometry(double nw, double nh);
  void setCornerRadius(double r);
  double getCornerRadius() const;
  void moveBy(double dx, double dy) override;
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;
};
