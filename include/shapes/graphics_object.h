// graphics_object.h
#pragma once
#include <QPainter>
#include <string>

class GraphicsObject {
 protected:
  double strokeWidth = 2.0;
  std::string fillColor = "blue";
  std::string strokeColor = "black";

 public:
  // destructor
  // virtual means to go to the actual object functions, and not just remain at
  // the GraphicsObject pointer
  virtual ~GraphicsObject() = default;

  // the virtual functions for drawing, converting to SVG, and whether the
  // selection contains my mouse (for editing later on)
  virtual void draw(QPainter& painter) const = 0;
  virtual std::string toSVG() const = 0;
  virtual bool contains(double x, double y) const = 0;

  // setters and getters for the things in protected
  // these live here, and no need to be overwritten by the children because
  // these are common to every shape
  void setStrokeColor(const std::string& color);
  std::string getStrokeColor() const;

  void setFillColor(const std::string& color);
  std::string getFillColor() const;

  void setStrokeWidth(double w);
  double getStrokeWidth() const;
};
