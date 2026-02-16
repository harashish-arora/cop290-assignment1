// canvas_clipboard.cpp
// cut/copy/paste/delete/clear commands
#include <algorithm>

#include "gui/canvas.h"
#include "tools/command.h"

// clipboard operations for cut/copy/paste/delete/clear commands
void Canvas::deleteSelected() {
  if (!selectedShape) return;
  auto removedShape = selectedShape;
  shapes.erase(std::remove(shapes.begin(), shapes.end(), removedShape),
               shapes.end());
  setSelectedShape(nullptr);
  pushCommand(std::make_unique<RemoveShapeCommand>(removedShape));
  update();
}

void Canvas::copySelected() {
  if (selectedShape) clipboard = selectedShape->clone();
}

// cut copies then deletes the selected shape
void Canvas::cutSelected() {
  if (!selectedShape) return;
  clipboard = selectedShape->clone();
  deleteSelected();
}

// paste creates a clone of the clipboard shape and adds it to the document
void Canvas::pasteAtCursor() {
  if (!clipboard) return;
  auto shape = clipboard->clone();
  QRectF box = shape->boundingBox();
  shape->moveBy(lastMousePos.x() - box.center().x(),
                lastMousePos.y() - box.center().y());
  shapes.push_back(shape);
  setSelectedShape(shape);
  pushCommand(std::make_unique<AddShapeCommand>(shape));
  update();
}

// clear all deletes all shapes and clears selection with a single command
void Canvas::clearAll() {
  if (shapes.empty()) return;
  auto clearCmd = std::make_unique<ClearAllCommand>(shapes, selectedShape);
  shapes.clear();
  setSelectedShape(nullptr);
  pushCommand(std::move(clearCmd));
  update();
}
