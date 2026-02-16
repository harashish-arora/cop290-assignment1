// idle state create cpp
// implementation for idle state create

#include "gui/canvas.h"
#include "shapes/circle.h"
#include "shapes/freehand.h"
#include "shapes/hexagon.h"
#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "shapes/rounded_rectangle.h"
#include "tools/creating_state.h"
#include "tools/shape_style_defaults.h"

// create preview shape based on current mode and creation defaults
// actual commit happens on mouse release in creating state
void startShapeCreation(Canvas* canvas, QPointF click) {
  auto defaults = getCreationDefaults();
  if (canvas->getMode() == ShapeMode::CIRCLE) {
    auto circle = std::make_shared<Circle>(click.x(), click.y(), 0);
    applyDefaultShapeStyle(circle);
    canvas->setPreviewShape(circle);
  } else if (canvas->getMode() == ShapeMode::HEXAGON) {
    auto hex = std::make_shared<Hexagon>(click.x(), click.y(), 0, 0);
    applyDefaultShapeStyle(hex);
    hex->setPointyTop(defaults.hexPointyTop);
    canvas->setPreviewShape(hex);
  } else if (canvas->getMode() == ShapeMode::LINE) {
    auto line =
        std::make_shared<Line>(click.x(), click.y(), click.x(), click.y());
    applyDefaultLineStyle(line);
    canvas->setPreviewShape(line);
  } else if (canvas->getMode() == ShapeMode::ROUNDED_RECT) {
    auto rr = std::make_shared<RoundedRectangle>(click.x(), click.y(), 0, 0);
    applyDefaultShapeStyle(rr);
    rr->setCornerRadius(defaults.cornerRadius);
    canvas->setPreviewShape(rr);
  } else if (canvas->getMode() == ShapeMode::FREEHAND) {
    auto fh = std::make_shared<Freehand>();
    applyDefaultLineStyle(fh);
    fh->addPoint(click.x(), click.y());
    canvas->setPreviewShape(fh);
  } else {
    auto rect = std::make_shared<Rectangle>(click.x(), click.y(), 0, 0);
    applyDefaultShapeStyle(rect);
    canvas->setPreviewShape(rect);
  }

  canvas->setState(std::make_unique<CreatingState>());
  canvas->update();
}
