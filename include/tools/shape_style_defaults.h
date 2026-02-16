// shape_style_defaults.h
// default fill/stroke presets for new shapes
#pragma once

#include <memory>
#include <string>

class GraphicsObject;

// struct to hold default properties for new shapes created by the user
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

// getter/setter for the global creation defaults
// helper functions to apply them
CreationDefaults getCreationDefaults();
void setCreationDefaults(const CreationDefaults& defaults);
std::string defaultShapeFillColor();
std::string defaultShapeStrokeColor();
std::string defaultLineFillColor();
void applyDefaultShapeStyle(const std::shared_ptr<GraphicsObject>& shape);
void applyDefaultLineStyle(const std::shared_ptr<GraphicsObject>& shape);
