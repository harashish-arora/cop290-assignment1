// text_shape_ops.cpp — TextShape move, clone, bounding box, accessors
#include <QFont>
#include <QFontMetricsF>

#include "shapes/text_shape.h"

void TextShape::moveBy(double dx, double dy) {
  x += dx;
  y += dy;
}

std::shared_ptr<GraphicsObject> TextShape::clone() const {
  auto copy = std::make_shared<TextShape>(x, y, text);
  copy->fontFamily = fontFamily;
  copy->fontSize = fontSize;
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

void TextShape::setFromBoundingBox(const QRectF& box) {
  QFont f(QString::fromStdString(fontFamily), fontSize);
  QFontMetricsF fm(f);
  x = box.x();
  y = box.y() + fm.ascent();
}

void TextShape::setText(const std::string& value) { text = value; }
const std::string& TextShape::getText() const { return text; }

void TextShape::setFontFamily(const std::string& family) {
  fontFamily = family;
}
const std::string& TextShape::getFontFamily() const { return fontFamily; }

void TextShape::setFontSize(int size) {
  if (size < 6) size = 6;
  if (size > 200) size = 200;
  fontSize = size;
}
int TextShape::getFontSize() const { return fontSize; }
