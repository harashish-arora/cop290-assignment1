// shape_style_defaults.h — default fill/stroke presets for new shapes
#pragma once

#include <memory>

class GraphicsObject;

void applyDefaultShapeStyle(const std::shared_ptr<GraphicsObject>& shape);
void applyDefaultLineStyle(const std::shared_ptr<GraphicsObject>& shape);
