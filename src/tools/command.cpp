// command cpp
// command pattern for undo redo

#include "tools/command.h"

#include <algorithm>

#include "gui/canvas.h"
#include "shapes/graphics_object.h"

Command::~Command() = default;

// addshapecommand
AddShapeCommand::AddShapeCommand(std::shared_ptr<GraphicsObject> s)
    : shape(std::move(s)) {}

// redo add shape by appending it again
void AddShapeCommand::redo(Canvas* c) {
  c->getShapes().push_back(shape);
  c->setSelectedShape(shape);
  c->update();
}

// undo add shape by removing the same shape instance
void AddShapeCommand::undo(Canvas* c) {
  auto& shapes = c->getShapes();
  shapes.erase(std::remove(shapes.begin(), shapes.end(), shape), shapes.end());
  if (c->getSelectedShape() == shape) c->setSelectedShape(nullptr);
  c->update();
}

// removeshapecommand
RemoveShapeCommand::RemoveShapeCommand(std::shared_ptr<GraphicsObject> s)
    : shape(std::move(s)) {}

// redo remove shape by erasing it from shape list
void RemoveShapeCommand::redo(Canvas* c) {
  auto& shapes = c->getShapes();
  shapes.erase(std::remove(shapes.begin(), shapes.end(), shape), shapes.end());
  if (c->getSelectedShape() == shape) c->setSelectedShape(nullptr);
  c->update();
}

// undo remove shape by re adding it
void RemoveShapeCommand::undo(Canvas* c) {
  c->getShapes().push_back(shape);
  c->setSelectedShape(shape);
  c->update();
}

// movecommand
MoveCommand::MoveCommand(std::shared_ptr<GraphicsObject> s, double dx,
                         double dy)
    : shape(std::move(s)), dx(dx), dy(dy) {}

// redo move by applying stored delta
void MoveCommand::redo(Canvas* c) {
  shape->moveBy(dx, dy);
  c->update();
}

// undo move by applying negative stored delta
void MoveCommand::undo(Canvas* c) {
  shape->moveBy(-dx, -dy);
  c->update();
}

// resizecommand
ResizeCommand::ResizeCommand(std::shared_ptr<GraphicsObject> s, QRectF oldBox,
                             QRectF newBox)
    : shape(std::move(s)), oldBox(oldBox), newBox(newBox) {}

// redo resize by restoring new box
void ResizeCommand::redo(Canvas* c) {
  shape->setFromBoundingBox(newBox);
  c->update();
}

// undo resize by restoring original box
void ResizeCommand::undo(Canvas* c) {
  shape->setFromBoundingBox(oldBox);
  c->update();
}

// clearallcommand
ClearAllCommand::ClearAllCommand(
    std::vector<std::shared_ptr<GraphicsObject>> shapes,
    std::shared_ptr<GraphicsObject> sel)
    : saved(std::move(shapes)), savedSelection(std::move(sel)) {}

// redo clear by emptying all shapes
void ClearAllCommand::redo(Canvas* c) {
  c->getShapes().clear();
  c->setSelectedShape(nullptr);
  c->update();
}

// undo clear by restoring saved list and selection
void ClearAllCommand::undo(Canvas* c) {
  c->getShapes() = saved;
  c->setSelectedShape(savedSelection);
  c->update();
}
