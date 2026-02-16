// shape_style_defaults.cpp
// global defaults for shape styles and convenience functions to apply them

#include "tools/shape_style_defaults.h"

#include "shapes/graphics_object.h"

namespace {
// global defaults used by properties panel and shape creation flow
CreationDefaults gCreationDefaults{"#80ffffff", "#ff000000", "transparent", 1.0,
                                   10.0,        false,       "Arial",       16};
}  // namespace

// return current defaults snapshot
CreationDefaults getCreationDefaults() { return gCreationDefaults; }

// update global defaults with basic clamping
void setCreationDefaults(const CreationDefaults& defaults) {
  gCreationDefaults = defaults;
  if (gCreationDefaults.strokeWidth < 1.0) gCreationDefaults.strokeWidth = 1.0;
  if (gCreationDefaults.cornerRadius < 0.0)
    gCreationDefaults.cornerRadius = 0.0;
  if (gCreationDefaults.fontSize < 1) gCreationDefaults.fontSize = 1;
}

// convenience getters used by panel initialization
std::string defaultShapeFillColor() { return gCreationDefaults.fillColor; }
std::string defaultShapeStrokeColor() { return gCreationDefaults.strokeColor; }
std::string defaultLineFillColor() { return gCreationDefaults.lineFillColor; }

// apply current defaults to fillable shapes
void applyDefaultShapeStyle(const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return;
  shape->setFillColor(defaultShapeFillColor());
  shape->setStrokeColor(defaultShapeStrokeColor());
  shape->setStrokeWidth(gCreationDefaults.strokeWidth);
}

// apply current defaults to line like shapes
void applyDefaultLineStyle(const std::shared_ptr<GraphicsObject>& shape) {
  if (!shape) return;
  shape->setFillColor(defaultLineFillColor());
  shape->setStrokeColor(defaultShapeStrokeColor());
  shape->setStrokeWidth(gCreationDefaults.strokeWidth);
}
