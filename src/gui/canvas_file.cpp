// canvas_file.cpp
// file handling for canvas - new/open/save/save as with unsaved changes prompt

#include <QFileDialog>
#include <QMessageBox>
#include <fstream>

#include "gui/canvas.h"
#include "gui/unsaved_changes_dialog.h"
#include "parse/svg_parser.h"

// save current document to current file path
// update saved snapshot after successful write
void Canvas::save() {
  if (currentFilePath.isEmpty()) {
    saveAs();
    return;
  }
  std::ofstream file(currentFilePath.toStdString());
  if (!file.is_open()) return;

  file << "<svg width=\"" << width() << "\" height=\"" << height()
       << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";

  for (const auto& shape : shapes) file << "  " << shape->toSVG() << "\n";

  file << "</svg>\n";

  savedXml = computeDocumentXml();
  syncModifiedState();
}

// ask user for a path, then call save
void Canvas::saveAs() {
  QString path = QFileDialog::getSaveFileName(this, "Save SVG", QString(),
                                              "SVG Files (*.svg)");
  if (path.isEmpty()) return;
  currentFilePath = path;
  save();
}

// open svg file and replace document contents
// reset undo/redo stacks and update saved snapshot
void Canvas::openFile() {
  static bool shownSvgLimitNote = false;
  if (!shownSvgLimitNote) {
    QMessageBox::information(
        this, "limited svg support",
        "this editor reads a restricted svg subset and is most reliable with svg files saved by this app");
    shownSvgLimitNote = true;
  }
  QString path = QFileDialog::getOpenFileName(this, "Open SVG", QString(),
                                              "SVG Files (*.svg)");
  if (path.isEmpty()) return;
  auto loaded = SvgParser::load(path.toStdString());
  if (loaded.empty()) {
    QMessageBox::warning(
        this, "open failed",
        "could not parse this svg with current subset support try an svg saved by this app");
    return;
  }
  shapes.clear();
  undoStack.clear();
  redoStack.clear();
  currentStateId = 0;
  nextStateId = 1;
  setSelectedShape(nullptr);
  shapes = std::move(loaded);
  currentFilePath = path;

  // update saved snapshot after loading new file
  savedXml = computeDocumentXml();
  syncModifiedState();
  update();
}

// create new blank document with unsaved changes prompt
void Canvas::newFile() {
  if (dirty) {
    auto choice = promptUnsavedChanges(
        this, "You have unsaved changes. Save before creating a new file?");
    if (choice == UnsavedChoice::Cancel) return;
    if (choice == UnsavedChoice::Save) save();
  }
  shapes.clear();
  undoStack.clear();
  redoStack.clear();
  currentStateId = 0;
  nextStateId = 1;
  setSelectedShape(nullptr);
  previewShape = nullptr;
  clipboard = nullptr;
  currentFilePath.clear();
  savedXml = computeDocumentXml();
  syncModifiedState();
  update();
}
