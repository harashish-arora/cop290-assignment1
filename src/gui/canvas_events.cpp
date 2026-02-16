// canvas_events.cpp — Double-click, key press, text editing input
#include <algorithm>

#include "gui/canvas.h"
#include "shapes/text_shape.h"
#include "tools/idle_state.h"

void Canvas::mouseDoubleClickEvent(QMouseEvent* e) {
  if (e->button() != Qt::LeftButton) return;
  QPointF click = e->position();
  for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
    auto txt = std::dynamic_pointer_cast<TextShape>(*it);
    if (txt && txt->contains(click.x(), click.y())) {
      setSelectedShape(*it);
      textEditing = true;
      update();
      return;
    }
  }
}

void Canvas::keyPressEvent(QKeyEvent* e) {
  if (textEditing) {
    auto txt = std::dynamic_pointer_cast<TextShape>(selectedShape);
    if (!txt) {
      textEditing = false;
      return;
    }
    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
      finalizeTextEditing();
      update();
      return;
    }
    if (e->key() == Qt::Key_Escape) {
      finalizeTextEditing();
      update();
      return;
    }
    if (e->key() == Qt::Key_Backspace) {
      std::string s = txt->getText();
      if (!s.empty()) s.pop_back();
      txt->setText(s);
      update();
      return;
    }
    QString input = e->text();
    if (!input.isEmpty() && input.at(0).unicode() >= 32) {
      txt->setText(txt->getText() + input.toStdString());
      update();
    }
    return;
  }
  currentState->handleKeyPress(this, e);
}

void Canvas::setState(std::unique_ptr<CanvasState> newState) {
  currentState = std::move(newState);
}

std::vector<std::shared_ptr<GraphicsObject>>& Canvas::getShapes() {
  return shapes;
}
std::shared_ptr<GraphicsObject>& Canvas::getSelectedShape() {
  return selectedShape;
}
std::shared_ptr<GraphicsObject>& Canvas::getPreviewShape() {
  return previewShape;
}
void Canvas::setSelectedShape(std::shared_ptr<GraphicsObject> shape) {
  selectedShape = std::move(shape);
  emit selectionChanged();
}
void Canvas::setPreviewShape(std::shared_ptr<GraphicsObject> shape) {
  previewShape = std::move(shape);
}

ShapeMode Canvas::getMode() const { return currentMode; }
void Canvas::setMode(ShapeMode mode) { currentMode = mode; }
void Canvas::beginTextEditing() { textEditing = true; }
void Canvas::endTextEditing() { textEditing = false; }
void Canvas::setTextDraftShape(std::shared_ptr<GraphicsObject> shape) {
  textDraftShape = std::move(shape);
}
QPointF Canvas::getStartPoint() const { return startPoint; }
void Canvas::setStartPoint(QPointF p) { startPoint = p; }
QPointF Canvas::getLastMousePos() const { return lastMousePos; }
void Canvas::setLastMousePos(QPointF p) { lastMousePos = p; }
