#include "shapes/graphics_object.h"
void GraphicsObject::setStrokeColor(const std::string& color) {
  strokeColor = color;
}

std::string GraphicsObject::getStrokeColor() const { return strokeColor; }

void GraphicsObject::setFillColor(const std::string& color) {
  fillColor = color;
}

std::string GraphicsObject::getFillColor() const { return fillColor; }

void GraphicsObject::setStrokeWidth(int w) { strokeWidth = w; }

int GraphicsObject::getStrokeWidth() const { return strokeWidth; }
