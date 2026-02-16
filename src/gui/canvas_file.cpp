// canvas_file.cpp — Save, open, new file operations
#include <QFileDialog>
#include <fstream>

#include "gui/canvas.h"
#include "gui/unsaved_changes_dialog.h"
#include "parse/svg_parser.h"

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

void Canvas::saveAs() {
  QString path = QFileDialog::getSaveFileName(this, "Save SVG", QString(),
                                              "SVG Files (*.svg)");
  if (path.isEmpty()) return;
  currentFilePath = path;
  save();
}

void Canvas::openFile() {
  QString path = QFileDialog::getOpenFileName(this, "Open SVG", QString(),
                                              "SVG Files (*.svg)");
  if (path.isEmpty()) return;
  auto loaded = SvgParser::load(path.toStdString());
  if (loaded.empty()) return;
  shapes.clear();
  undoStack.clear();
  redoStack.clear();
  currentStateId = 0;
  nextStateId = 1;
  setSelectedShape(nullptr);
  shapes = std::move(loaded);
  currentFilePath = path;
  // record saved XML after loading file
  savedXml = computeDocumentXml();
  syncModifiedState();
  update();
}

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
