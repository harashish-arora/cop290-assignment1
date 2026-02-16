// canvas_history.cpp
// undo/redo implementation for canvas

#include "gui/canvas.h"

// push a command onto undo stack and clear redo stack
// called for every user action that changes document state
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

// modified state tracking based on comparing current document XML with saved
// snapshot
bool Canvas::isModified() const { return dirty; }
QString Canvas::getFilePath() const { return currentFilePath; }
bool Canvas::isHistoryReplayInProgress() const {
  return historyReplayInProgress;
}

// undo by moving one entry from undo stack to redo stack
// toggle replay flag to avoid recording undo/redo commands during history
// replay
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

// similar to undo above but in reverse direction
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

// serialize current shape list into comparable xml text
// used to check whether document is modified compared to last saved state
QString Canvas::computeDocumentXml() const {
  QString data;
  data.reserve(shapes.size() * 32);
  for (const auto& s : shapes) data += QString::fromStdString(s->toSVG());
  return data;
}

// compare current xml with saved xml and emit signal on change
void Canvas::syncModifiedState() {
  QString cur = computeDocumentXml();
  bool modifiedNow = (cur != savedXml);
  if (dirty == modifiedNow) return;
  dirty = modifiedNow;
  emit modifiedChanged();
}
