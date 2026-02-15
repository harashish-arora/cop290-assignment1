// resizing_state.h — State for dragging a handle to resize a shape
#pragma once
#include <QPointF>
#include <vector>

#include "tools/canvas_state.h"
#include "tools/handle_helpers.h"

class ResizingState : public CanvasState {
 public:
  ResizingState(HandleType handle, double left, double top, double right,
                double bottom);

  void handleMousePress(Canvas* canvas, QMouseEvent* event) override;
  void handleMouseMove(Canvas* canvas, QMouseEvent* event) override;
  void handleMouseRelease(Canvas* canvas, QMouseEvent* event) override;

  // Call after construction to snapshot freehand points
  void snapshotFreehandPoints(const std::vector<QPointF>& pts);

 private:
  HandleType activeHandle;

  // The 4 edges saved at drag start — only the dragged ones change
  double anchorLeft, anchorTop, anchorRight, anchorBottom;

  // Original bounding box for undo
  QRectF oldBox;

  // Snapshot of freehand points at drag start
  std::vector<QPointF> origFreehandPts;
  QRectF origFreehandBox;
};
