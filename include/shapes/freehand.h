// freehand.h — Freehand sketch shape (polyline of points)
#pragma once
#include <vector>

#include "shapes/graphics_object.h"

class Freehand : public GraphicsObject {
 private:
  std::vector<QPointF> points;

 public:
  Freehand();

  void addPoint(double x, double y);
  const std::vector<QPointF>& getPoints() const;

  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;
  QRectF boundingBox() const override;

  void moveBy(double dx, double dy) override;
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;

  // Scale points into raw left/top/right/bottom (right<left = horizontal flip)
  void scaleToBox(double left, double top, double right, double bottom);
};
