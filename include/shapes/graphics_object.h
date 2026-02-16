// graphics_object.h
#pragma once
#include <QPainter>
#include <QRectF>
#include <string>

class GraphicsObject {
 protected:
  double width;
  double height;
  double strokeWidth;
  std::string fillColor;
  std::string strokeColor;

  // Convert a stored color string to SVG attributes.
  // Handles #AARRGGBB → attr="#RRGGBB" attr-opacity="0..1"
  // and "transparent" → attr="none"
  static std::string svgColorAttr(const std::string& prefix,
                                  const std::string& color);

 public:
  // destructor
  // virtual means to go to the actual object functions, and not just remain at
  // the GraphicsObject pointer
  GraphicsObject();

  virtual ~GraphicsObject();

  // the virtual functions for drawing, converting to SVG, and whether the
  // selection contains my mouse (for editing later on)
  virtual void draw(QPainter& painter) const = 0;
  virtual std::string toSVG() const = 0;
  virtual bool contains(double x, double y) const = 0;

  // setters and getters for colors
  // these live here, and no need to be overwritten by the children because
  // these are common to every shape
  void setStrokeColor(const std::string& color);
  void setFillColor(const std::string& color);
  void setStrokeWidth(double w);
  std::string getStrokeColor() const;
  std::string getFillColor() const;
  double getStrokeWidth() const;

  // boundary boxes
  virtual QRectF boundingBox() const = 0;

  // deep-copy this shape (polymorphic)
  virtual std::shared_ptr<GraphicsObject> clone() const = 0;

  // translate the shape by (dx, dy)
  virtual void moveBy(double dx, double dy) = 0;

  // restore geometry from a bounding box (for undo/redo)
  virtual void setFromBoundingBox(const QRectF& box) = 0;

  // setters and getters for boundary boxes and size
  void setSize(double w, double h);
  double getWidth() const;
  double getHeight() const;
};
