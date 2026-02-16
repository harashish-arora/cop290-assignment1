// app_style.cpp
// global application stylesheet

#include "gui/app_style.h"

// return one global Qt stylesheet string for the whole application
QString styleSheet() {
  return R"(
    QMainWindow { background: #f0f0f0; }
    QMenuBar {
      background: #e8e8e8;
      border-bottom: 1px solid #c8c8c8;
      padding: 1px 0px;
    }
    QMenuBar::item {
      padding: 4px 8px;
      background: transparent; border-radius: 3px;
    }
    QMenuBar::item:selected { background: #d0d0d0; }
    QMenu {
      background: #f5f5f5;
      border: 1px solid #c0c0c0; padding: 4px;
    }
    QMenu::item {
      padding: 4px 24px 4px 8px; border-radius: 2px;
    }
    QMenu::item:selected {
      background: #0078d4; color: white;
    }
    QMenu::separator {
      height: 1px; background: #d0d0d0; margin: 4px 8px;
    }
    QLabel { color: #333; font-size: 11px; }
    QSlider::groove:horizontal {
      height: 4px; background: #ccc; border-radius: 2px;
    }
    QSlider::handle:horizontal {
      background: #888; border: 1px solid #666;
      width: 10px; height: 10px;
      margin: -4px 0; border-radius: 5px;
    }
    QSlider::handle:horizontal:hover { background: #666; }
    QSpinBox {
      background: white; color: #222;
      border: 1px solid #bbb; border-radius: 3px;
      padding: 1px 2px; font-size: 11px;
    }
    QSpinBox:focus { border: 1px solid #0078d4; }
    QMessageBox { background: #f5f5f5; }
    QMessageBox QLabel { color: #222; font-size: 13px; }
    QMessageBox QPushButton {
      background: #e0e0e0; color: #222;
      border: 1px solid #aaa; border-radius: 4px;
      padding: 5px 16px; font-size: 12px; min-width: 70px;
    }
    QMessageBox QPushButton:hover { background: #d0d0d0; }
    QMessageBox QPushButton:pressed { background: #c0c0c0; }
  )";
}
