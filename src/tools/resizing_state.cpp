// resizing state cpp
// fsm state for dragging a handle to resize a shape

#include "tools/resizing_state.h"

#include <algorithm>

#include "gui/canvas.h"
#include "shapes/line.h"
#include "tools/command.h"
#include "tools/idle_state.h"

// constructor captures active handle and original box used for undo
ResizingState::ResizingState(HandleType handle, double left, double top,
                             double right, double bottom)
    : activeHandle(handle),
      anchorLeft(left),
      anchorTop(top),
      anchorRight(right),
      anchorBottom(bottom),
      oldBox(left, top, right - left, bottom - top) {}

// save freehand points before resize so shape can be remapped proportionally
void ResizingState::snapshotFreehandPoints(const std::vector<QPointF>& pts) {
  origFreehandPts = pts;
  if (!pts.empty()) {
    double minX = pts[0].x(), maxX = minX;
    double minY = pts[0].y(), maxY = minY;
    for (auto& p : pts) {
      minX = std::min(minX, p.x());
      maxX = std::max(maxX, p.x());
      minY = std::min(minY, p.y());
      maxY = std::max(maxY, p.y());
    }
    origFreehandBox = QRectF(minX, minY, maxX - minX, maxY - minY);
  }
}

// press already handled in idle state
void ResizingState::handleMousePress(Canvas*, QMouseEvent*) {}

// update selected shape while dragging active handle
void ResizingState::handleMouseMove(Canvas* canvas, QMouseEvent* event) {
  if (!(event->buttons() & Qt::LeftButton)) return;
  auto& selected = canvas->getSelectedShape();
  if (!selected) return;

  QPointF pos = event->position();

  // line uses endpoint handles not box resize
  auto line = std::dynamic_pointer_cast<Line>(selected);
  if (line) {
    if (activeHandle == HandleType::LINE_START)
      line->setEndpoints(pos.x(), pos.y(), line->getX2(), line->getY2());
    else if (activeHandle == HandleType::LINE_END)
      line->setEndpoints(line->getX1(), line->getY1(), pos.x(), pos.y());
    canvas->setLastMousePos(pos);
    canvas->update();
    return;
  }

  applyResize(canvas, pos);
}

// push one resize command if geometry changed then return to idle
void ResizingState::handleMouseRelease(Canvas* canvas, QMouseEvent* event) {
  if (event->button() != Qt::LeftButton) return;
  auto& sel = canvas->getSelectedShape();
  if (sel) {
    QRectF newBox = sel->boundingBox();
    if (newBox != oldBox)
      canvas->pushCommand(std::make_unique<ResizeCommand>(sel, oldBox, newBox));
  }
  canvas->setState(std::make_unique<IdleState>());
  canvas->update();
}
