// main_window.cpp
// this defines the top level window along with the menu bar

#include "gui/main_window.h"

#include <QFileInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "gui/canvas.h"
#include "gui/properties_panel.h"
#include "gui/tool_bar.h"
#include "gui/unsaved_changes_dialog.h"

// build main window layout and wire cross widget signals
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  auto* container = new QWidget(this);
  auto* outerLayout = new QHBoxLayout(container);
  outerLayout->setContentsMargins(0, 0, 0, 0);
  outerLayout->setSpacing(0);

  canvas = new Canvas(container);
  panel = new PropertiesPanel(canvas, container);
  toolBar = new ToolBar(canvas, container);

  // right column with canvas and properties panel stacked vertically
  auto* rightCol = new QVBoxLayout;
  rightCol->setContentsMargins(0, 0, 0, 0);
  rightCol->setSpacing(0);
  rightCol->addWidget(canvas, 1);
  rightCol->addWidget(panel, 0);

  // main layout
  outerLayout->addLayout(rightCol, 1);
  outerLayout->addWidget(toolBar, 0);

  // light background for toolbar and panel areas
  container->setStyleSheet("background: #f0f0f0;");
  setCentralWidget(container);
  setWindowTitle("ProjectInkscape");
  resize(1200, 700);
  createMenus();

  connect(canvas, &Canvas::selectionChanged, panel,
          &PropertiesPanel::refreshFromSelection);
  connect(canvas, &Canvas::modifiedChanged, this,
          &MainWindow::updateWindowTitle);
}

// expose canvas pointer for tests and integration points
Canvas* MainWindow::getCanvas() const { return canvas; }

// close action with unsaved changes prompt
void MainWindow::closeFile() {
  if (canvas->isModified()) {
    auto choice = promptUnsavedChanges(
        this, "You have unsaved changes. Save before closing?");
    if (choice == UnsavedChoice::Cancel) return;
    if (choice == UnsavedChoice::Save) canvas->save();
  }
  close();
}

// keep title synced with current file and modified flag
void MainWindow::updateWindowTitle() {
  QString title = "ProjectInkscape";
  QString path = canvas->getFilePath();
  if (!path.isEmpty()) {
    QFileInfo fi(path);
    title += " — " + fi.fileName();
  }
  if (canvas->isModified()) title += " *";
  setWindowTitle(title);
}
