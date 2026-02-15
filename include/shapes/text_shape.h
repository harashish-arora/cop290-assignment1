#pragma once

#include <string>

#include "shapes/graphics_object.h"

class TextShape : public GraphicsObject {
 public:
  TextShape(double x, double y, const std::string& text = "");

  void draw(QPainter& painter) const override;
  std::string toSVG() const override;
  bool contains(double x, double y) const override;
  QRectF boundingBox() const override;

  void moveBy(double dx, double dy) override;
  std::shared_ptr<GraphicsObject> clone() const override;
  void setFromBoundingBox(const QRectF& box) override;

  void setText(const std::string& value);
  const std::string& getText() const;

  void setFontFamily(const std::string& family);
  const std::string& getFontFamily() const;

  void setFontSize(int size);
  int getFontSize() const;

 private:
  static std::string escapeXml(const std::string& s);

  double x;
  double y;  // baseline y
  std::string text;
  std::string fontFamily = "Arial";
  int fontSize = 16;
};
