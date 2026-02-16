// freehand.h
// Freehand polyline shape (sequence of points)
#pragma once
#include <vector>

#include "shapes/graphics_object.h"

class Freehand : public GraphicsObject {
 private:
  std::vector<QPointF> points;  // ordered list of points composing the stroke

 public:
  Freehand();  // default constructor for an empty freehand stroke

  void addPoint(double x, double y);  // append a point
  const std::vector<QPointF>& getPoints() const;

  // drawing, SVG conversion, hit testing, and bounding box overrides
  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;
  QRectF boundingBox() const override;

  // update geometry and position
  void moveBy(double dx, double dy) override;
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;

  // re-map points to fit inside the provided box
  void scaleToBox(double left, double top, double right, double bottom);
};
