// rectangle.h
#pragma once
#include "shapes/graphics_object.h"

class Rectangle : public GraphicsObject {
 public:
  // Constructor
  Rectangle(double x, double y, double w, double h);

  // Overrides
  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double mouseX, double mouseY) const override;

  // NEW: The mandatory bounding box function
  QRectF boundingBox() const override;

  // Setters
  void setGeometry(double nw, double nh);  // sets width/height
  void moveBy(double dx, double dy) override;

  // Clone (deep copy)
  std::shared_ptr<GraphicsObject> clone() const override;

  // Restore from bounding box
  void setFromBoundingBox(const QRectF& box) override;

 private:
  double x;
  double y;
  // width and height are deleted because for the specific case of rectangle, it
  // will inherit its height and width directly from the parent
};
