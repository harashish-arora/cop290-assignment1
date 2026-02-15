// command.h — Command pattern for undo/redo
#pragma once
#include <QRectF>
#include <memory>

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
