// rectangle.h
// Rectangle shape class definition
#pragma once
#include "shapes/graphics_object.h"

class Rectangle : public GraphicsObject {
 private:
  // position for rectangle
  double x;
  double y;
  // width and height are stored via the bounding box

 public:
  Rectangle(double x, double y, double w, double h);  // constructor

  // drawing, SVG conversion, hit testing, and bounding box overrides
  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double mouseX, double mouseY) const override;
  QRectF boundingBox() const override;

  // update position and geometry
  void setGeometry(double nw, double nh);  // sets width/height
  void moveBy(double dx, double dy) override;

  // deep copy and restoring from box
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;
};
