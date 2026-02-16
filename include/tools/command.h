// command.h — Command pattern for undo/redo
#pragma once
#include <QRectF>
#include <memory>
#include <string>
#include <vector>

class Canvas;
class GraphicsObject;

class Command {
 public:
  virtual ~Command() = default;
  virtual void undo(Canvas* canvas) = 0;
  virtual void redo(Canvas* canvas) = 0;
};

// Adding a shape (create / paste)
class AddShapeCommand : public Command {
  std::shared_ptr<GraphicsObject> shape;

 public:
  explicit AddShapeCommand(std::shared_ptr<GraphicsObject> s);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};

// Removing a shape (delete / cut)
class RemoveShapeCommand : public Command {
  std::shared_ptr<GraphicsObject> shape;

 public:
  explicit RemoveShapeCommand(std::shared_ptr<GraphicsObject> s);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};

// Moving a shape by (dx, dy)
class MoveCommand : public Command {
  std::shared_ptr<GraphicsObject> shape;
  double dx, dy;

 public:
  MoveCommand(std::shared_ptr<GraphicsObject> s, double dx, double dy);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};

// Resizing a shape (stores old and new bounding boxes)
class ResizeCommand : public Command {
  std::shared_ptr<GraphicsObject> shape;
  QRectF oldBox, newBox;

 public:
  ResizeCommand(std::shared_ptr<GraphicsObject> s, QRectF oldBox,
                QRectF newBox);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};

// Editable properties shared across shapes plus shape-specific extras.
struct ShapePropertyState {
  std::string fillColor;
  std::string strokeColor;
  double strokeWidth = 1.0;

  bool hasCornerRadius = false;
  double cornerRadius = 0.0;

  bool hasPointyTop = false;
  bool pointyTop = false;

  bool hasTextStyle = false;
  std::string fontFamily;
  int fontSize = 16;
  bool hasTextContent = false;
  std::string textContent;

  bool operator==(const ShapePropertyState& other) const {
    return fillColor == other.fillColor && strokeColor == other.strokeColor &&
           strokeWidth == other.strokeWidth &&
           hasCornerRadius == other.hasCornerRadius &&
           cornerRadius == other.cornerRadius &&
           hasPointyTop == other.hasPointyTop && pointyTop == other.pointyTop &&
           hasTextStyle == other.hasTextStyle &&
           fontFamily == other.fontFamily && fontSize == other.fontSize &&
           hasTextContent == other.hasTextContent &&
           textContent == other.textContent;
  }
  bool operator!=(const ShapePropertyState& other) const {
    return !(*this == other);
  }
};

// Undoable property change for one shape.
class ShapePropertyCommand : public Command {
  std::shared_ptr<GraphicsObject> shape;
  ShapePropertyState before;
  ShapePropertyState after;

  void applyState(const ShapePropertyState& state, Canvas* canvas);

 public:
  ShapePropertyCommand(std::shared_ptr<GraphicsObject> shape,
                       ShapePropertyState before, ShapePropertyState after);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};

// Clearing all shapes (undoable)
class ClearAllCommand : public Command {
  std::vector<std::shared_ptr<GraphicsObject>> saved;
  std::shared_ptr<GraphicsObject> savedSelection;

 public:
  ClearAllCommand(std::vector<std::shared_ptr<GraphicsObject>> shapes,
                  std::shared_ptr<GraphicsObject> sel);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};
