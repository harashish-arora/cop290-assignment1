// text_shape_ops.cpp
// text shape operations for move, clone and resize by bounding box

#include <QFont>
#include <QFontMetricsF>

#include "shapes/text_shape.h"

// translate text position by offset
void TextShape::moveBy(double dx, double dy) {
  x += dx;
  y += dy;
}

// deep copy text shape with geometry, text and font for clipboard and commands
std::shared_ptr<GraphicsObject> TextShape::clone() const {
  auto copy = std::make_shared<TextShape>(x, y, text);
  copy->fontFamily = fontFamily;
  copy->fontSize = fontSize;
  copy->setFillColor(getFillColor());
  copy->setStrokeColor(getStrokeColor());
  copy->setStrokeWidth(getStrokeWidth());
  return copy;
}

// align text anchor to top left of bounding box while keeping baseline offset
void TextShape::setFromBoundingBox(const QRectF& box) {
  QFont f(QString::fromStdString(fontFamily), fontSize);
  QFontMetricsF fm(f);
  x = box.x();
  y = box.y() + fm.ascent();
}

// text content getters and setters
void TextShape::setText(const std::string& value) { text = value; }
const std::string& TextShape::getText() const { return text; }

// font family getters and setters
void TextShape::setFontFamily(const std::string& family) {
  fontFamily = family;
}
const std::string& TextShape::getFontFamily() const { return fontFamily; }

// font size setter with clamp range used by properties panel
void TextShape::setFontSize(int size) {
  if (size < 6) size = 6;
  if (size > 200) size = 200;
  fontSize = size;
}

int TextShape::getFontSize() const { return fontSize; }
