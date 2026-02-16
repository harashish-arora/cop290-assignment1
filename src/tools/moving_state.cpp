// moving state cpp
// fsm state for dragging to move a selected shape

#include "tools/moving_state.h"

#include "gui/canvas.h"
#include "tools/command.h"
#include "tools/idle_state.h"

// no extra press handling needed in moving state
void MovingState::handleMousePress(Canvas*, QMouseEvent*) {}

// move selected shape using mouse delta from last recorded position
void MovingState::handleMouseMove(Canvas* canvas, QMouseEvent* event) {
  if (!(event->buttons() & Qt::LeftButton)) return;

  auto& selected = canvas->getSelectedShape();
  if (!selected) return;
  canvas->setCursor(Qt::ClosedHandCursor);

  QPointF current = event->position();
  QPointF last = canvas->getLastMousePos();
  double dx = current.x() - last.x();
  double dy = current.y() - last.y();

  selected->moveBy(dx, dy);

  totalDx += dx;
  totalDy += dy;
  canvas->setLastMousePos(current);
  canvas->update();
}

// on release store one move command with total drag delta then return to idle
void MovingState::handleMouseRelease(Canvas* canvas, QMouseEvent* event) {
  if (event->button() != Qt::LeftButton) return;
  auto& sel = canvas->getSelectedShape();
  if (sel && (totalDx != 0 || totalDy != 0))
    canvas->pushCommand(std::make_unique<MoveCommand>(sel, totalDx, totalDy));
  canvas->setCursor(sel ? Qt::SizeAllCursor : Qt::ArrowCursor);
  canvas->setState(std::make_unique<IdleState>());
  canvas->update();
}
