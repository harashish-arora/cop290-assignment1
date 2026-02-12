#include <QApplication>
#include <QLabel>
#include <QPainter>

#include "shapes/graphics_object.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  QLabel label("Hello Qt from Homebrew!");
  label.resize(300, 100);
  label.show();

  return app.exec();
}
