// main_window.cpp — Constructor, close, title update
#include "gui/main_window.h"

#include <QFileInfo>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>

#include "gui/canvas.h"
#include "gui/properties_panel.h"
#include "gui/tool_bar.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  auto* container = new QWidget(this);
  auto* outerLayout = new QHBoxLayout(container);
  outerLayout->setContentsMargins(0, 0, 0, 0);
  outerLayout->setSpacing(0);

  canvas = new Canvas(container);
  panel = new PropertiesPanel(canvas, container);
  toolBar = new ToolBar(canvas, container);

  auto* rightCol = new QVBoxLayout;
  rightCol->setContentsMargins(0, 0, 0, 0);
  rightCol->setSpacing(0);
  rightCol->addWidget(canvas, 1);
  rightCol->addWidget(panel, 0);

  outerLayout->addLayout(rightCol, 1);
  outerLayout->addWidget(toolBar, 0);

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

Canvas* MainWindow::getCanvas() const { return canvas; }

void MainWindow::closeFile() {
  if (canvas->isModified()) {
    QMessageBox box(this);
    box.setIcon(QMessageBox::NoIcon);
    box.setWindowTitle("Unsaved Changes");
    box.setText("You have unsaved changes. Save before closing?");
    auto* saveBtn = box.addButton("Save", QMessageBox::AcceptRole);
    box.addButton("Don't Save", QMessageBox::DestructiveRole);
    auto* cancelBtn = box.addButton("Cancel", QMessageBox::RejectRole);
    box.setDefaultButton(cancelBtn);
    box.exec();
    auto* clicked = box.clickedButton();
    if (!clicked || clicked == cancelBtn) return;
    if (clicked == saveBtn) canvas->save();
  }
  close();
}

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
