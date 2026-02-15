// main_window.h — Top-level window with menu bar
#pragma once
#include <QMainWindow>

class Canvas;
class PropertiesPanel;
class ToolBar;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);

  Canvas* getCanvas() const;

 private slots:
  void closeFile();
  void updateWindowTitle();

 private:
  Canvas* canvas;
  PropertiesPanel* panel;
  ToolBar* toolBar;

  void createMenus();
};
