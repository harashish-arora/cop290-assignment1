// canvas_clipboard.cpp — Cut/copy/paste, delete, undo/redo, text finalize
#include <algorithm>

#include "gui/canvas.h"
#include "shapes/text_shape.h"
#include "tools/command.h"

void Canvas::finalizeTextEditing() {
  if (!textEditing) return;
  auto txt = std::dynamic_pointer_cast<TextShape>(selectedShape);
  textEditing = false;
  if (!txt) {
    textDraftShape = nullptr;
    return;
  }
  bool isDraft = (textDraftShape && textDraftShape == selectedShape);
  if (isDraft) {
    if (txt->getText().empty()) {
      shapes.erase(std::remove(shapes.begin(), shapes.end(), selectedShape),
                   shapes.end());
      setSelectedShape(nullptr);
    } else {
      pushCommand(std::make_unique<AddShapeCommand>(selectedShape));
    }
    textDraftShape = nullptr;
  }
}

void Canvas::deleteSelected() {
  if (!selectedShape) return;
  pushCommand(std::make_unique<RemoveShapeCommand>(selectedShape));
  shapes.erase(std::remove(shapes.begin(), shapes.end(), selectedShape),
               shapes.end());
  setSelectedShape(nullptr);
  update();
}

void Canvas::copySelected() {
  if (selectedShape) clipboard = selectedShape->clone();
}

void Canvas::cutSelected() {
  if (!selectedShape) return;
  clipboard = selectedShape->clone();
  deleteSelected();
}

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

void Canvas::pushCommand(std::unique_ptr<Command> cmd) {
  undoStack.push_back(std::move(cmd));
  redoStack.clear();
  if (!dirty) {
    dirty = true;
    emit modifiedChanged();
  }
}

bool Canvas::isModified() const { return dirty; }
QString Canvas::getFilePath() const { return currentFilePath; }

void Canvas::undo() {
  if (undoStack.empty()) return;
  auto cmd = std::move(undoStack.back());
  undoStack.pop_back();
  cmd->undo(this);
  redoStack.push_back(std::move(cmd));
}

void Canvas::redo() {
  if (redoStack.empty()) return;
  auto cmd = std::move(redoStack.back());
  redoStack.pop_back();
  cmd->redo(this);
  undoStack.push_back(std::move(cmd));
}

void Canvas::clearAll() {
  if (shapes.empty()) return;
  pushCommand(std::make_unique<ClearAllCommand>(shapes, selectedShape));
  shapes.clear();
  setSelectedShape(nullptr);
  update();
}
