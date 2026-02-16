// shape_style_defaults.h — default fill/stroke presets for new shapes
#pragma once

#include <memory>

#include "shapes/graphics_object.h"

inline void applyDefaultShapeStyle(const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return;
  shape->setFillColor("#80ffffff");
  shape->setStrokeColor("#ff000000");
}

inline void applyDefaultLineStyle(const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return;
  shape->setFillColor("transparent");
  shape->setStrokeColor("#ff000000");
}
