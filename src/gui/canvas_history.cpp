// canvas_history.cpp — command history and modified-state tracking
#include "gui/canvas.h"

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
bool Canvas::isHistoryReplayInProgress() const { return historyReplayInProgress; }

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

QString Canvas::computeDocumentXml() const {
  QString data;
  data.reserve(shapes.size() * 32);
  for (const auto& s : shapes) data += QString::fromStdString(s->toSVG());
  return data;
}

void Canvas::syncModifiedState() {
  QString cur = computeDocumentXml();
  bool modifiedNow = (cur != savedXml);
  if (dirty == modifiedNow) return;
  dirty = modifiedNow;
  emit modifiedChanged();
}
