// main_window_menus.cpp — File and Edit menu creation
#include <QMenuBar>

#include "gui/canvas.h"
#include "gui/main_window.h"

void MainWindow::createMenus() {
  QMenu* fileMenu = menuBar()->addMenu("&File");

  QAction* newAction = fileMenu->addAction("New");
  newAction->setShortcut(QKeySequence::New);
  connect(newAction, &QAction::triggered, canvas, &Canvas::newFile);

  QAction* openAction = fileMenu->addAction("Open...");
  openAction->setShortcut(QKeySequence::Open);
  connect(openAction, &QAction::triggered, canvas, &Canvas::openFile);
  fileMenu->addSeparator();

  QAction* saveAction = fileMenu->addAction("Save");
  saveAction->setShortcut(QKeySequence::Save);
  connect(saveAction, &QAction::triggered, canvas, &Canvas::save);

  QAction* saveAsAction = fileMenu->addAction("Save As...");
  saveAsAction->setShortcut(QKeySequence::SaveAs);
  connect(saveAsAction, &QAction::triggered, canvas, &Canvas::saveAs);
  fileMenu->addSeparator();

  QAction* closeAction = fileMenu->addAction("Close");
  closeAction->setShortcut(QKeySequence::Close);
  connect(closeAction, &QAction::triggered, this, &MainWindow::closeFile);

  QMenu* editMenu = menuBar()->addMenu("&Edit");

  QAction* undoAction = editMenu->addAction("Undo");
  undoAction->setShortcut(QKeySequence::Undo);
  connect(undoAction, &QAction::triggered, canvas, &Canvas::undo);

  QAction* redoAction = editMenu->addAction("Redo");
  redoAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));
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
