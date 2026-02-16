// shape_style_defaults.cpp — default fill/stroke presets for new shapes
#include "tools/shape_style_defaults.h"

#include "shapes/graphics_object.h"

namespace {
CreationDefaults gCreationDefaults{"#80ffffff", "#ff000000", "transparent", 1.0,
                                  10.0, false, "Arial", 16};
}

CreationDefaults getCreationDefaults() { return gCreationDefaults; }

void setCreationDefaults(const CreationDefaults& defaults) {
  gCreationDefaults = defaults;
  if (gCreationDefaults.strokeWidth < 1.0) gCreationDefaults.strokeWidth = 1.0;
  if (gCreationDefaults.cornerRadius < 0.0) gCreationDefaults.cornerRadius = 0.0;
  if (gCreationDefaults.fontSize < 1) gCreationDefaults.fontSize = 1;
}

std::string defaultShapeFillColor() { return gCreationDefaults.fillColor; }
std::string defaultShapeStrokeColor() { return gCreationDefaults.strokeColor; }
std::string defaultLineFillColor() { return gCreationDefaults.lineFillColor; }

void applyDefaultShapeStyle(const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return;
  shape->setFillColor(defaultShapeFillColor());
  shape->setStrokeColor(defaultShapeStrokeColor());
  shape->setStrokeWidth(gCreationDefaults.strokeWidth);
}

void applyDefaultLineStyle(const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return;
  shape->setFillColor(defaultLineFillColor());
  shape->setStrokeColor(defaultShapeStrokeColor());
  shape->setStrokeWidth(gCreationDefaults.strokeWidth);
}
