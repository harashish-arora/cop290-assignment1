// graphics_object.cpp
#include "shapes/graphics_object.h"

// strokeColor
void GraphicsObject::setStrokeColor(const std::string& color) {
  strokeColor = color;
}

std::string GraphicsObject::getStrokeColor() const { return strokeColor; }

// fillColor

void GraphicsObject::setFillColor(const std::string& color) {
  strokeColor = color;
}

std::string GraphicsObject::getFillColor() const { return fillColor; }

// strokeWidth

void GraphicsObject::setStrokeWidth(double w) { strokeWidth = w; }

double GraphicsObject::getStrokeWidth() const { return strokeWidth; }
