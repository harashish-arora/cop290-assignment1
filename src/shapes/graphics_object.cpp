// graphics_object.cpp
// base class for drawable shapes

#include "shapes/graphics_object.h"

#include <QColor>
#include <iomanip>
#include <sstream>

// constructor for graphics object with default style and zero size
GraphicsObject::GraphicsObject()
    : width(0),
      height(0),
      strokeWidth(1.0),
      fillColor("green"),
      strokeColor("black") {}

GraphicsObject::~GraphicsObject() = default;

// stroke colour accessors
void GraphicsObject::setStrokeColor(const std::string& color) {
  strokeColor = color;
}
std::string GraphicsObject::getStrokeColor() const { return strokeColor; }

// fill colour accessors
void GraphicsObject::setFillColor(const std::string& color) {
  fillColor = color;
}
std::string GraphicsObject::getFillColor() const { return fillColor; }

// stroke width accessors
void GraphicsObject::setStrokeWidth(double w) { strokeWidth = w; }
double GraphicsObject::getStrokeWidth() const { return strokeWidth; }

// size setters and getters
void GraphicsObject::setSize(double w, double h) {
  width = w;
  height = h;
}
double GraphicsObject::getWidth() const { return width; }
double GraphicsObject::getHeight() const { return height; }

std::string GraphicsObject::svgColorAttr(const std::string& prefix,
                                         const std::string& color) {
  if (color == "transparent" || color == "none") {
    return prefix + "=\"none\"";
  }
  QColor c(color.c_str());
  if (!c.isValid()) return prefix + "=\"" + color + "\"";

  // build rrggbb with no opacity if fully opaque, otherwise add separate
  // opacity attribute
  std::ostringstream oss;
  oss << prefix << "=\"" << c.name(QColor::HexRgb).toStdString() << "\"";

  // add opacity attribute if not fully opaque
  if (c.alpha() < 255) {
    double opacity = c.alphaF();
    oss << " " << prefix << "-opacity=\"" << std::fixed << std::setprecision(3)
        << opacity << "\"";
  }
  return oss.str();
}
