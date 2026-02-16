// resizing_state.h
// fsm state for dragging a handle to resize a shape
#pragma once
#include <QPointF>
#include <vector>

#include "tools/canvas_state.h"
#include "tools/handle_helpers.h"

// active when the user is dragging a resize handle on a shape
class ResizingState : public CanvasState {
 private:
  HandleType activeHandle;

  // 4 edges saved at drag start, only dragged ones change
  double anchorLeft, anchorTop, anchorRight, anchorBottom;

  // Original bounding box for undo
  QRectF oldBox;

  // snapshot of freehand points at start of resize
  std::vector<QPointF> origFreehandPts;
  QRectF origFreehandBox;

 public:
  ResizingState(HandleType handle, double left, double top, double right,
                double bottom);

  void handleMousePress(Canvas* canvas, QMouseEvent* event) override;
  void handleMouseMove(Canvas* canvas, QMouseEvent* event) override;
  void handleMouseRelease(Canvas* canvas, QMouseEvent* event) override;

  // call after construction to snapshot freehand points
  void snapshotFreehandPoints(const std::vector<QPointF>& pts);

  // apply non-line resizing
  void applyResize(Canvas* canvas, QPointF pos);
};
