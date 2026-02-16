// canvas_file.cpp — Save, open, new file operations
#include <QAbstractButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <fstream>

#include "gui/canvas.h"
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
  dirty = false;
  emit modifiedChanged();
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
  setSelectedShape(nullptr);
  shapes = std::move(loaded);
  currentFilePath = path;
  dirty = false;
  emit modifiedChanged();
  update();
}

void Canvas::newFile() {
  if (dirty) {
    QMessageBox box(this);
    box.setIcon(QMessageBox::NoIcon);
    box.setWindowTitle("Unsaved Changes");
    box.setText("You have unsaved changes. Save before creating a new file?");
    auto* saveBtn = box.addButton("Save", QMessageBox::AcceptRole);
    box.addButton("Don't Save", QMessageBox::DestructiveRole);
    auto* cancelBtn = box.addButton("Cancel", QMessageBox::RejectRole);
    box.setDefaultButton(cancelBtn);
    box.exec();
    QAbstractButton* clicked = box.clickedButton();
    if (!clicked || clicked == cancelBtn) return;
    if (clicked == saveBtn) save();
  }
  shapes.clear();
  undoStack.clear();
  redoStack.clear();
  setSelectedShape(nullptr);
  previewShape = nullptr;
  clipboard = nullptr;
  currentFilePath.clear();
  dirty = false;
  emit modifiedChanged();
  update();
}
