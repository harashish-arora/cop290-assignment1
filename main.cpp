// main cpp
// entry point for the qt application

#include <QApplication>

#include "gui/app_style.h"  // global style helper
#include "gui/main_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  // create qt application object

  app.setStyleSheet(styleSheet());  // apply app wide stylesheet

  MainWindow window;
  window.show();
  return app.exec();
}
