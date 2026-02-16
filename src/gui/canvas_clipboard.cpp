// canvas_clipboard.cpp
// cut, copy, paste, delete operations and undo/redo
#include <QLineEdit>
#include <algorithm>

#include "gui/canvas.h"
#include "shapes/text_shape.h"
#include "tools/command.h"

void Canvas::finalizeTextEditing() {
  if (!textEditing) return;
  auto txt = std::dynamic_pointer_cast<TextShape>(selectedShape);
  if (txt && textEditor) txt->setText(textEditor->text().toStdString());
  endTextEditing();
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
  } else {
    ShapePropertyState before;
    before.fillColor = txt->getFillColor();
    before.strokeColor = txt->getStrokeColor();
    before.strokeWidth = txt->getStrokeWidth();
    before.hasTextStyle = true;
    before.fontFamily = txt->getFontFamily();
    before.fontSize = txt->getFontSize();
    before.hasTextContent = true;
    before.textContent = textBeforeEditing;

    auto after = before;
    after.textContent = txt->getText();
    if (after != before) {
      pushCommand(
          std::make_unique<ShapePropertyCommand>(selectedShape, before, after));
    }
  }

  update();
}

QString Canvas::computeDocumentXml() const {
  QString data;
  data.reserve(shapes.size() * 32);
  for (const auto& s : shapes) data += QString::fromStdString(s->toSVG());
  return data;
}

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
  if (historyReplayInProgress) return;
  HistoryEntry entry;
  entry.command = std::move(cmd);
  entry.prevStateId = currentStateId;
  entry.nextStateId = nextStateId++;
  undoStack.push_back(std::move(entry));
  redoStack.clear();
  currentStateId = undoStack.back().nextStateId;
  syncModifiedState();
}

bool Canvas::isModified() const { return dirty; }
QString Canvas::getFilePath() const { return currentFilePath; }
bool Canvas::isHistoryReplayInProgress() const {
  return historyReplayInProgress;
}

void Canvas::undo() {
  if (undoStack.empty()) return;
  auto entry = std::move(undoStack.back());
  undoStack.pop_back();
  historyReplayInProgress = true;
  entry.command->undo(this);
  historyReplayInProgress = false;
  currentStateId = entry.prevStateId;
  redoStack.push_back(std::move(entry));
  syncModifiedState();
}

void Canvas::redo() {
  if (redoStack.empty()) return;
  auto entry = std::move(redoStack.back());
  redoStack.pop_back();
  historyReplayInProgress = true;
  entry.command->redo(this);
  historyReplayInProgress = false;
  currentStateId = entry.nextStateId;
  undoStack.push_back(std::move(entry));
  syncModifiedState();
}

void Canvas::syncModifiedState() {
  QString cur = computeDocumentXml();
  bool modifiedNow = (cur != savedXml);
  if (dirty == modifiedNow) return;
  dirty = modifiedNow;
  emit modifiedChanged();
}

void Canvas::clearAll() {
  if (shapes.empty()) return;
  auto clearCmd = std::make_unique<ClearAllCommand>(shapes, selectedShape);
  shapes.clear();
  setSelectedShape(nullptr);
  pushCommand(std::move(clearCmd));
  update();
}
