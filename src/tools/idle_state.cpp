// idle state cpp
// fsm state for when nothing is being dragged

#include "tools/idle_state.h"

#include "gui/canvas.h"
#include "shapes/freehand.h"
#include "shapes/text_shape.h"
#include "tools/handle_helpers.h"
#include "tools/moving_state.h"
#include "tools/resizing_state.h"
#include "tools/shape_style_defaults.h"

// defined in idle state create cpp
void startShapeCreation(Canvas* canvas, QPointF click);

// handle press in idle state
// this decides if user is resizing moving editing text or creating a new shape
void IdleState::handleMousePress(Canvas* canvas, QMouseEvent* event) {
  if (event->button() != Qt::LeftButton) return;
  QPointF click = event->position();
  canvas->setStartPoint(click);
  canvas->setLastMousePos(click);
  auto& selected = canvas->getSelectedShape();
  if (selected) {
    // clicking a handle switches to resizing state
    HandleType handle = getHandleAt(click, selected);
    if (handle != HandleType::NONE) {
      QRectF box = selected->boundingBox();
      auto state = std::make_unique<ResizingState>(
          handle, box.left(), box.top(), box.right(), box.bottom());
      auto fh = std::dynamic_pointer_cast<Freehand>(selected);
      if (fh) state->snapshotFreehandPoints(fh->getPoints());
      canvas->setState(std::move(state));
      canvas->update();
      return;
    }
  }

  // clicking a shape switches to moving state
  auto& shapes = canvas->getShapes();
  for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
    if ((*it)->contains(click.x(), click.y())) {
      canvas->setSelectedShape(*it);
      canvas->endTextEditing();
      canvas->setCursor(Qt::ClosedHandCursor);
      canvas->setState(std::make_unique<MovingState>());
      canvas->update();
      return;
    }
  }

  // text mode creates a draft text shape and starts inline editing
  if (canvas->getMode() == ShapeMode::TEXT) {
    auto txt = std::make_shared<TextShape>(click.x(), click.y(), "");
    auto defaults = getCreationDefaults();
    applyDefaultShapeStyle(txt);
    txt->setFontFamily(defaults.fontFamily);
    txt->setFontSize(defaults.fontSize);
    canvas->getShapes().push_back(txt);
    canvas->setSelectedShape(txt);
    canvas->setTextDraftShape(txt);
    canvas->beginTextEditing();
    canvas->setState(std::make_unique<IdleState>());
    canvas->update();
    return;
  }

  // empty click clears selection
  canvas->setSelectedShape(nullptr);

  // select mode stops here no new shape creation
  if (canvas->getMode() == ShapeMode::SELECT) {
    canvas->update();
    return;
  }

  // shape modes create preview shape and go to creating state
  startShapeCreation(canvas, click);
}

// update cursor feedback for handles and selectable shapes
void IdleState::handleMouseMove(Canvas* canvas, QMouseEvent* event) {
  QPointF pos = event->position();
  auto& selected = canvas->getSelectedShape();
  if (selected) {
    HandleType hover = getHandleAt(pos, selected);
    if (hover != HandleType::NONE) {
      updateCursorForHandle(canvas, hover);
      return;
    }
  }
  auto& shapes = canvas->getShapes();
  for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
    if ((*it)->contains(pos.x(), pos.y())) {
      canvas->setCursor(Qt::SizeAllCursor);
      return;
    }
  }
  canvas->setCursor(Qt::ArrowCursor);
}

void IdleState::handleMouseRelease(Canvas*, QMouseEvent*) {}

// keyboard delete removes selected shape
void IdleState::handleKeyPress(Canvas* canvas, QKeyEvent* event) {
  if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
    canvas->deleteSelected();
  }
}
