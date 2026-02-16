// command.cpp — Implementations of undo/redo commands
#include "tools/command.h"

#include <algorithm>

#include "gui/canvas.h"
#include "shapes/graphics_object.h"
#include "shapes/hexagon.h"
#include "shapes/rounded_rectangle.h"
#include "shapes/text_shape.h"

// --- AddShapeCommand ---
AddShapeCommand::AddShapeCommand(std::shared_ptr<GraphicsObject> s)
    : shape(std::move(s)) {}

void AddShapeCommand::redo(Canvas* c) {
  c->getShapes().push_back(shape);
  c->setSelectedShape(shape);
  c->update();
}

void AddShapeCommand::undo(Canvas* c) {
  auto& shapes = c->getShapes();
  shapes.erase(std::remove(shapes.begin(), shapes.end(), shape), shapes.end());
  if (c->getSelectedShape() == shape) c->setSelectedShape(nullptr);
  c->update();
}

// --- RemoveShapeCommand ---
RemoveShapeCommand::RemoveShapeCommand(std::shared_ptr<GraphicsObject> s)
    : shape(std::move(s)) {}

void RemoveShapeCommand::redo(Canvas* c) {
  auto& shapes = c->getShapes();
  shapes.erase(std::remove(shapes.begin(), shapes.end(), shape), shapes.end());
  if (c->getSelectedShape() == shape) c->setSelectedShape(nullptr);
  c->update();
}

void RemoveShapeCommand::undo(Canvas* c) {
  c->getShapes().push_back(shape);
  c->setSelectedShape(shape);
  c->update();
}

// --- MoveCommand ---
MoveCommand::MoveCommand(std::shared_ptr<GraphicsObject> s, double dx,
                         double dy)
    : shape(std::move(s)), dx(dx), dy(dy) {}

void MoveCommand::redo(Canvas* c) {
  shape->moveBy(dx, dy);
  c->update();
}

void MoveCommand::undo(Canvas* c) {
  shape->moveBy(-dx, -dy);
  c->update();
}

// --- ResizeCommand ---
ResizeCommand::ResizeCommand(std::shared_ptr<GraphicsObject> s, QRectF oldBox,
                             QRectF newBox)
    : shape(std::move(s)), oldBox(oldBox), newBox(newBox) {}

void ResizeCommand::redo(Canvas* c) {
  shape->setFromBoundingBox(newBox);
  c->update();
}

void ResizeCommand::undo(Canvas* c) {
  shape->setFromBoundingBox(oldBox);
  c->update();
}

// --- ShapePropertyCommand ---
ShapePropertyCommand::ShapePropertyCommand(std::shared_ptr<GraphicsObject> shape,
                                           ShapePropertyState before,
                                           ShapePropertyState after)
    : shape(std::move(shape)), before(std::move(before)), after(std::move(after)) {}

void ShapePropertyCommand::applyState(const ShapePropertyState& state,
                                      Canvas* canvas) {
  if (!shape) return;

  shape->setFillColor(state.fillColor);
  shape->setStrokeColor(state.strokeColor);
  shape->setStrokeWidth(state.strokeWidth);

  if (state.hasCornerRadius) {
    if (auto rr = std::dynamic_pointer_cast<RoundedRectangle>(shape))
      rr->setCornerRadius(state.cornerRadius);
  }

  if (state.hasPointyTop) {
    if (auto hex = std::dynamic_pointer_cast<Hexagon>(shape))
      hex->setPointyTop(state.pointyTop);
  }

  if (state.hasTextStyle) {
    if (auto text = std::dynamic_pointer_cast<TextShape>(shape)) {
      text->setFontFamily(state.fontFamily);
      text->setFontSize(state.fontSize);
    }
  }
  if (state.hasTextContent) {
    if (auto text = std::dynamic_pointer_cast<TextShape>(shape))
      text->setText(state.textContent);
  }

  canvas->update();
}

void ShapePropertyCommand::undo(Canvas* canvas) { applyState(before, canvas); }

void ShapePropertyCommand::redo(Canvas* canvas) { applyState(after, canvas); }

// --- ClearAllCommand ---
ClearAllCommand::ClearAllCommand(
    std::vector<std::shared_ptr<GraphicsObject>> shapes,
    std::shared_ptr<GraphicsObject> sel)
    : saved(std::move(shapes)), savedSelection(std::move(sel)) {}

void ClearAllCommand::redo(Canvas* c) {
  c->getShapes().clear();
  c->setSelectedShape(nullptr);
  c->update();
}

void ClearAllCommand::undo(Canvas* c) {
  c->getShapes() = saved;
  c->setSelectedShape(savedSelection);
  c->update();
}
