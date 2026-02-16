// shape_style_defaults.cpp — default fill/stroke presets for new shapes
#include "tools/shape_style_defaults.h"

#include "shapes/graphics_object.h"

void applyDefaultShapeStyle(const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return;
  shape->setFillColor("#80ffffff");
  shape->setStrokeColor("#ff000000");
}

void applyDefaultLineStyle(const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return;
  shape->setFillColor("transparent");
  shape->setStrokeColor("#ff000000");
}
