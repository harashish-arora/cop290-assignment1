// command.h
// Command pattern for undo/redo
#pragma once
#include <QRectF>
#include <memory>
#include <vector>

class Canvas;
class GraphicsObject;

class Command {
 public:
  virtual ~Command();
  virtual void undo(Canvas* canvas) = 0;
  virtual void redo(Canvas* canvas) = 0;
};

// adding a shape to the canvas
class AddShapeCommand : public Command {
 private:
  std::shared_ptr<GraphicsObject> shape;

 public:
  explicit AddShapeCommand(std::shared_ptr<GraphicsObject> s);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};

// removing a shape by deleting or cutting from canvas
class RemoveShapeCommand : public Command {
 private:
  std::shared_ptr<GraphicsObject> shape;

 public:
  explicit RemoveShapeCommand(std::shared_ptr<GraphicsObject> s);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};

// moving a shape by (dx, dy)
class MoveCommand : public Command {
 private:
  std::shared_ptr<GraphicsObject> shape;
  double dx, dy;

 public:
  MoveCommand(std::shared_ptr<GraphicsObject> s, double dx, double dy);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};

// resizing a shape (stores old and new bounding boxes)
class ResizeCommand : public Command {
 private:
  std::shared_ptr<GraphicsObject> shape;
  QRectF oldBox, newBox;

 public:
  ResizeCommand(std::shared_ptr<GraphicsObject> s, QRectF oldBox,
                QRectF newBox);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};

// clearing all shapes from the canvas (used for new and clear all)
class ClearAllCommand : public Command {
 private:
  std::vector<std::shared_ptr<GraphicsObject>> saved;
  std::shared_ptr<GraphicsObject> savedSelection;

 public:
  ClearAllCommand(std::vector<std::shared_ptr<GraphicsObject>> shapes,
                  std::shared_ptr<GraphicsObject> sel);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};
