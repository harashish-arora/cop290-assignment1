// main_window.cpp — MainWindow with File and Edit menus
#include "gui/main_window.h"

#include <QFileInfo>
#include <QHBoxLayout>
#include <QMenuBar>
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

void MainWindow::createMenus() {
  // --- File menu ---
  QMenu* fileMenu = menuBar()->addMenu("&File");

  QAction* newAction = fileMenu->addAction("New");
  newAction->setShortcut(QKeySequence::New);  // Cmd+N / Ctrl+N
  connect(newAction, &QAction::triggered, canvas, &Canvas::newFile);

  QAction* openAction = fileMenu->addAction("Open...");
  openAction->setShortcut(QKeySequence::Open);  // Cmd+O / Ctrl+O
  connect(openAction, &QAction::triggered, canvas, &Canvas::openFile);

  fileMenu->addSeparator();

  QAction* saveAction = fileMenu->addAction("Save");
  saveAction->setShortcut(QKeySequence::Save);  // Cmd+S / Ctrl+S
  connect(saveAction, &QAction::triggered, canvas, &Canvas::save);

  QAction* saveAsAction = fileMenu->addAction("Save As...");
  saveAsAction->setShortcut(
      QKeySequence::SaveAs);  // Cmd+Shift+S / Ctrl+Shift+S
  connect(saveAsAction, &QAction::triggered, canvas, &Canvas::saveAs);

  fileMenu->addSeparator();

  QAction* closeAction = fileMenu->addAction("Close");
  closeAction->setShortcut(QKeySequence::Close);  // Cmd+W / Ctrl+W
  connect(closeAction, &QAction::triggered, this, &MainWindow::closeFile);

  // --- Edit menu ---
  QMenu* editMenu = menuBar()->addMenu("&Edit");

  QAction* undoAction = editMenu->addAction("Undo");
  undoAction->setShortcut(QKeySequence::Undo);  // Ctrl+Z / Cmd+Z
  connect(undoAction, &QAction::triggered, canvas, &Canvas::undo);

  QAction* redoAction = editMenu->addAction("Redo");
  redoAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));  // Cmd+Y
  connect(redoAction, &QAction::triggered, canvas, &Canvas::redo);

  editMenu->addSeparator();

  QAction* cutAction = editMenu->addAction("Cut");
  cutAction->setShortcut(QKeySequence::Cut);
  connect(cutAction, &QAction::triggered, canvas, &Canvas::cutSelected);

  QAction* copyAction = editMenu->addAction("Copy");
  copyAction->setShortcut(QKeySequence::Copy);
  connect(copyAction, &QAction::triggered, canvas, &Canvas::copySelected);

  QAction* pasteAction = editMenu->addAction("Paste");
  pasteAction->setShortcut(QKeySequence::Paste);
  connect(pasteAction, &QAction::triggered, canvas, &Canvas::pasteAtCursor);

  editMenu->addSeparator();

  QAction* clearAction = editMenu->addAction("Clear All");
  connect(clearAction, &QAction::triggered, canvas, &Canvas::clearAll);
}

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
