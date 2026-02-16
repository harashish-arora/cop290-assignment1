// idle_state_create.cpp — Creates preview shapes for drawing modes
#include "gui/canvas.h"
#include "shapes/circle.h"
#include "shapes/freehand.h"
#include "shapes/hexagon.h"
#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "shapes/rounded_rectangle.h"
#include "tools/creating_state.h"

void startShapeCreation(Canvas* canvas, QPointF click) {
  if (canvas->getMode() == ShapeMode::CIRCLE) {
    auto circle = std::make_shared<Circle>(click.x(), click.y(), 0);
    circle->setFillColor("#80ffffff");
    circle->setStrokeColor("#ff000000");
    canvas->setPreviewShape(circle);
  } else if (canvas->getMode() == ShapeMode::HEXAGON) {
    auto hex = std::make_shared<Hexagon>(click.x(), click.y(), 0, 0);
    hex->setFillColor("#80ffffff");
    hex->setStrokeColor("#ff000000");
    canvas->setPreviewShape(hex);
  } else if (canvas->getMode() == ShapeMode::LINE) {
    auto line =
        std::make_shared<Line>(click.x(), click.y(), click.x(), click.y());
    line->setFillColor("transparent");
    line->setStrokeColor("#ff000000");
    canvas->setPreviewShape(line);
  } else if (canvas->getMode() == ShapeMode::ROUNDED_RECT) {
    auto rr = std::make_shared<RoundedRectangle>(click.x(), click.y(), 0, 0);
    rr->setFillColor("#80ffffff");
    rr->setStrokeColor("#ff000000");
    canvas->setPreviewShape(rr);
  } else if (canvas->getMode() == ShapeMode::FREEHAND) {
    auto fh = std::make_shared<Freehand>();
    fh->addPoint(click.x(), click.y());
    canvas->setPreviewShape(fh);
  } else {
    auto rect = std::make_shared<Rectangle>(click.x(), click.y(), 0, 0);
    rect->setFillColor("#80ffffff");
    rect->setStrokeColor("#ff000000");
    canvas->setPreviewShape(rect);
  }
  canvas->setState(std::make_unique<CreatingState>());
  canvas->update();
}
