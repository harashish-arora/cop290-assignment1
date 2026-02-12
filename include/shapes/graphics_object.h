#pragma once
#include <QPainter>
#include <string>

class GraphicsObject {
 public:
  virtual ~GraphicsObject() = default;

  // children override these
  virtual void draw(QPainter& painter) const = 0;
  virtual std::string toSVG() const = 0;
  virtual bool contains(int x, int y) const = 0;

  // these are implemented in src.shapes.graphics_object.cpp
  void setStrokeColor(const std::string& color);
  std::string getStrokeColor() const;

  void setFillColor(const std::string& color);
  std::string getFillColor() const;

  void setStrokeWidth(int w);
  int getStrokeWidth() const;

 protected:
  std::string strokeColor = "black";
  std::string fillColor = "none";
  int strokeWidth = 1;
};
