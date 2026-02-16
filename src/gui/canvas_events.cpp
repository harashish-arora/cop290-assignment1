// canvas_events.cpp — Double-click, key press, text editing input
#include <QFont>
#include <QLineEdit>
#include <algorithm>
#include <cmath>

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
      beginTextEditing(true);
      update();
      return;
    }
  }
}

void Canvas::keyPressEvent(QKeyEvent* e) {
  if (textEditing && e->key() == Qt::Key_Escape) {
    finalizeTextEditing();
    setFocus();
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
void Canvas::beginTextEditing(bool selectAll) {
  auto txt = std::dynamic_pointer_cast<TextShape>(selectedShape);
  if (!txt || !textEditor) {
    textEditing = false;
    return;
  }

  textEditing = true;
  textBeforeEditing = txt->getText();

  QFont font(QString::fromStdString(txt->getFontFamily()), txt->getFontSize());
  textEditor->setFont(font);
  textEditor->setText(QString::fromStdString(txt->getText()));

  QRectF box = txt->boundingBox();
  int x = static_cast<int>(std::floor(box.x())) - 2;
  int y = static_cast<int>(std::floor(box.y())) - 2;
  int w = std::max(120, static_cast<int>(std::ceil(box.width())) + 12);
  int h = std::max(24, static_cast<int>(std::ceil(box.height())) + 8);
  textEditor->setGeometry(x, y, w, h);

  textEditor->show();
  textEditor->raise();
  textEditor->setFocus();

  if (selectAll) {
    textEditor->selectAll();
  } else {
    textEditor->setCursorPosition(textEditor->text().size());
  }
}
void Canvas::endTextEditing() {
  textEditing = false;
  if (textEditor) textEditor->hide();
}
void Canvas::setTextDraftShape(std::shared_ptr<GraphicsObject> shape) {
  textDraftShape = std::move(shape);
}
QPointF Canvas::getStartPoint() const { return startPoint; }
void Canvas::setStartPoint(QPointF p) { startPoint = p; }
QPointF Canvas::getLastMousePos() const { return lastMousePos; }
void Canvas::setLastMousePos(QPointF p) { lastMousePos = p; }
