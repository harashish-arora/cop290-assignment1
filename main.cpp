#include <QApplication>

#include "gui/app_style.h"
#include "gui/main_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setStyleSheet(appStyleSheet());

  MainWindow window;
  window.show();
  return app.exec();
}
