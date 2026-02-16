// main_window.h
// this defines the top level window along with the menu bar
#pragma once
#include <QMainWindow>

// forward declarations
class Canvas;           // in canvas.h
class PropertiesPanel;  // in properties_panel.h
class ToolBar;          // in tool_bar.h

class MainWindow : public QMainWindow {
  Q_OBJECT
  // macro for signal and slot features

 private:  // pointers to child widgets which are in main window
  Canvas* canvas;
  PropertiesPanel* panel;
  ToolBar* toolBar;

  void createMenus();  // internal setup helper for menus

 private slots:              // internal event handlers
  void closeFile();          // event handler for close action
  void updateWindowTitle();  // updating titlebar with filename and status

 public:
  explicit MainWindow(QWidget* parent = nullptr);

  Canvas* getCanvas() const;
};
