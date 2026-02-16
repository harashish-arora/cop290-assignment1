// shape_style_defaults.h — default fill/stroke presets for new shapes
#pragma once

#include <memory>
#include <string>

class GraphicsObject;

struct CreationDefaults {
  std::string fillColor;
  std::string strokeColor;
  std::string lineFillColor;
  double strokeWidth;
  double cornerRadius;
  bool hexPointyTop;
  std::string fontFamily;
  int fontSize;
};

CreationDefaults getCreationDefaults();
void setCreationDefaults(const CreationDefaults& defaults);
std::string defaultShapeFillColor();
std::string defaultShapeStrokeColor();
std::string defaultLineFillColor();
void applyDefaultShapeStyle(const std::shared_ptr<GraphicsObject>& shape);
void applyDefaultLineStyle(const std::shared_ptr<GraphicsObject>& shape);
