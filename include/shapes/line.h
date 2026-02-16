// line.h
// Line segment shape defined by two endpoints
#pragma once
#include "shapes/graphics_object.h"

class Line : public GraphicsObject {
 private:
  double x1, y1, x2, y2;  // endpoints

 public:
  // construct a line from (x1,y1) to (x2,y2).
  Line(double x1, double y1, double x2, double y2);

  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;  // hit test near segment
  QRectF boundingBox() const override;

  // geometry accessors and mutators
  void setEndpoints(double x1, double y1, double x2, double y2);
  double getX1() const;
  double getY1() const;
  double getX2() const;
  double getY2() const;

  void moveBy(double dx, double dy) override;

  // clone and bounding box use the endpoints, not width/height
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;
};
