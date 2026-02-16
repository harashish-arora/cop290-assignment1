// graphics_object.h
// Base class for drawable shapes
#pragma once
#include <QPainter>
#include <QRectF>
#include <string>

class GraphicsObject {
 protected:
  // basic visual properties of the bounding box and colorus
  double width;
  double height;
  double strokeWidth;
  std::string fillColor;
  std::string strokeColor;

  // convert stored colour strings to SVG
  static std::string svgColorAttr(const std::string& prefix,
                                  const std::string& color);

 public:
  GraphicsObject();
  virtual ~GraphicsObject();

  // polymorphic interface for drawing, hit-testing and SVG conversion
  virtual void draw(QPainter& painter) const = 0;
  virtual std::string toSVG() const = 0;
  virtual bool contains(double x, double y) const = 0;

  // colour and stroke accessors (common implementation).
  void setStrokeColor(const std::string& color);
  void setFillColor(const std::string& color);
  void setStrokeWidth(double w);
  std::string getStrokeColor() const;
  std::string getFillColor() const;
  double getStrokeWidth() const;

  // geometry accessors and mutators
  // width and height are stored via the bounding box
  virtual QRectF boundingBox() const = 0;
  virtual std::shared_ptr<GraphicsObject> clone() const = 0;  // deep copy
  virtual void moveBy(double dx, double dy) = 0;
  virtual void setFromBoundingBox(const QRectF& box) = 0;

  void setSize(double w, double h);
  double getWidth() const;
  double getHeight() const;
};
