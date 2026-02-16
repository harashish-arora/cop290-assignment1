// text_shape.h
// TextShape represents a single-line text object with font properties
#pragma once

#include <string>

#include "shapes/graphics_object.h"

class TextShape : public GraphicsObject {
 private:
  // helper to escape special characters in XML output
  static std::string escapeXml(const std::string& s);

  double x;  // x position (left)
  double y;  // y position (baseline)
  std::string text;
  std::string fontFamily = "Arial";
  int fontSize = 16;

 public:
  TextShape(double x, double y, const std::string& text = "");

  // render text using QPainter, convert to SVG <text> element, and hit test
  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;
  QRectF boundingBox() const override;

  // update position and geometry
  void moveBy(double dx, double dy) override;
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;

  // text and font accessors
  void setText(const std::string& value);
  const std::string& getText() const;

  void setFontFamily(const std::string& family);
  const std::string& getFontFamily() const;

  void setFontSize(int size);
  int getFontSize() const;
};
