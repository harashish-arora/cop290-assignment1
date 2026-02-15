// idle_state.cpp — Handles clicks when idle (select, start create/move/resize)
#include "tools/idle_state.h"

#include "gui/canvas.h"
#include "shapes/circle.h"
#include "shapes/freehand.h"
#include "shapes/hexagon.h"
#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "shapes/rounded_rectangle.h"
#include "shapes/text_shape.h"
#include "tools/command.h"
#include "tools/creating_state.h"
#include "tools/handle_helpers.h"
#include "tools/moving_state.h"
#include "tools/resizing_state.h"

void IdleState::handleMousePress(Canvas* canvas, QMouseEvent* event) {
  if (event->button() != Qt::LeftButton) return;
  QPointF click = event->position();
  canvas->setStartPoint(click);
  canvas->setLastMousePos(click);
  auto& selected = canvas->getSelectedShape();
  if (selected) {
    HandleType handle = getHandleAt(click, selected);
    if (handle != HandleType::NONE) {
      QRectF box = selected->boundingBox();
      auto state = std::make_unique<ResizingState>(
          handle, box.left(), box.top(), box.right(), box.bottom());
      // Snapshot freehand points before resizing starts
      auto fh = std::dynamic_pointer_cast<Freehand>(selected);
      if (fh) state->snapshotFreehandPoints(fh->getPoints());
      canvas->setState(std::move(state));
      canvas->update();
      return;
    }
  }

  auto& shapes = canvas->getShapes();
  for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
    if ((*it)->contains(click.x(), click.y())) {
      canvas->setSelectedShape(*it);
      canvas->endTextEditing();
      canvas->setState(std::make_unique<MovingState>());
      canvas->update();
      return;
    }
  }

  if (canvas->getMode() == ShapeMode::TEXT) {
    auto txt = std::make_shared<TextShape>(click.x(), click.y(), "");
    txt->setFillColor("#280078d4");
    txt->setStrokeColor("#ff000000");
    canvas->getShapes().push_back(txt);
    canvas->setSelectedShape(txt);
    canvas->setTextDraftShape(txt);
    canvas->beginTextEditing();
    canvas->setState(std::make_unique<IdleState>());
    canvas->update();
    return;
  }

  canvas->setSelectedShape(nullptr);
  if (canvas->getMode() == ShapeMode::CIRCLE) {
    auto circle = std::make_shared<Circle>(click.x(), click.y(), 0);
    circle->setFillColor("transparent");
    circle->setStrokeColor("black");
    canvas->setPreviewShape(circle);
  } else if (canvas->getMode() == ShapeMode::HEXAGON) {
    auto hex = std::make_shared<Hexagon>(click.x(), click.y(), 0, 0);
    hex->setFillColor("transparent");
    hex->setStrokeColor("black");
    canvas->setPreviewShape(hex);
  } else if (canvas->getMode() == ShapeMode::LINE) {
    auto line =
        std::make_shared<Line>(click.x(), click.y(), click.x(), click.y());
    line->setFillColor("transparent");
    line->setStrokeColor("black");
    canvas->setPreviewShape(line);
  } else if (canvas->getMode() == ShapeMode::ROUNDED_RECT) {
    auto rr = std::make_shared<RoundedRectangle>(click.x(), click.y(), 0, 0);
    rr->setFillColor("transparent");
    rr->setStrokeColor("black");
    canvas->setPreviewShape(rr);
  } else if (canvas->getMode() == ShapeMode::FREEHAND) {
    auto fh = std::make_shared<Freehand>();
    fh->addPoint(click.x(), click.y());
    canvas->setPreviewShape(fh);
  } else {
    auto rect = std::make_shared<Rectangle>(click.x(), click.y(), 0, 0);
    rect->setFillColor("transparent");
    rect->setStrokeColor("black");
    canvas->setPreviewShape(rect);
  }
  canvas->setState(std::make_unique<CreatingState>());
  canvas->update();
}

void IdleState::handleMouseMove(Canvas* canvas, QMouseEvent* event) {
  auto& selected = canvas->getSelectedShape();
  if (selected) {
    HandleType hover = getHandleAt(event->position(), selected);
    updateCursorForHandle(canvas, hover);
  } else {
    canvas->setCursor(Qt::ArrowCursor);
  }
}
void IdleState::handleMouseRelease(Canvas*, QMouseEvent*) {}
void IdleState::handleKeyPress(Canvas* canvas, QKeyEvent* event) {
  if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
    canvas->deleteSelected();
  }
}
