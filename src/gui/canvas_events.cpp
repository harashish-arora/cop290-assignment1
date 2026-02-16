// canvas_events.cpp
// implementation for canvas events

#include <QFont>
#include <QLineEdit>
#include <algorithm>
#include <cmath>

#include "gui/canvas.h"
#include "shapes/text_shape.h"
#include "tools/idle_state.h"

// double clicking on a text shape starts inline text editing
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

// escape key cancels text editing, otherwise delegate to current state
void Canvas::keyPressEvent(QKeyEvent* e) {
  if (textEditing && e->key() == Qt::Key_Escape) {
    finalizeTextEditing();
    setFocus();
    return;
  }
  currentState->handleKeyPress(this, e);
}

// switch the fsm state to idle on mouse press if not handled by current state
void Canvas::setState(std::unique_ptr<CanvasState> newState) {
  currentState = std::move(newState);
}

// expose mutable shape list for commands and interaction states
std::vector<std::shared_ptr<GraphicsObject>>& Canvas::getShapes() {
  return shapes;
}

// expose mutable selected shape pointer
std::shared_ptr<GraphicsObject>& Canvas::getSelectedShape() {
  return selectedShape;
}

// expose mutable preview shape pointer
std::shared_ptr<GraphicsObject>& Canvas::getPreviewShape() {
  return previewShape;
}

// update selected shape and notify listeners like the properties panel
void Canvas::setSelectedShape(std::shared_ptr<GraphicsObject> shape) {
  selectedShape = std::move(shape);
  emit selectionChanged();
}

// update preview shape used while creating shapes
void Canvas::setPreviewShape(std::shared_ptr<GraphicsObject> shape) {
  previewShape = std::move(shape);
}

// read active interaction mode
ShapeMode Canvas::getMode() const { return currentMode; }

// set active interaction mode from toolbar actions
void Canvas::setMode(ShapeMode mode) { currentMode = mode; }

// place and focus inline editor on top of selected text shape
void Canvas::beginTextEditing(bool selectAll) {
  auto txt = std::dynamic_pointer_cast<TextShape>(selectedShape);
  if (!txt || !textEditor) {
    textEditing = false;
    return;
  }

  textEditing = true;
  textBeforeEditing = txt->getText();

  // mirror text shape properties in the editor widget
  QFont font(QString::fromStdString(txt->getFontFamily()), txt->getFontSize());
  textEditor->setFont(font);
  textEditor->setText(QString::fromStdString(txt->getText()));

  // position editor on top of text shape with some padding
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

// hide editor and apply text changes to the shape
void Canvas::endTextEditing() {
  textEditing = false;
  if (textEditor) textEditor->hide();
}

// remember whether selected text shape is a new draft shape
void Canvas::setTextDraftShape(std::shared_ptr<GraphicsObject> shape) {
  textDraftShape = std::move(shape);
}

// geometry getters and setters used by interaction states
QPointF Canvas::getStartPoint() const { return startPoint; }
void Canvas::setStartPoint(QPointF p) { startPoint = p; }
QPointF Canvas::getLastMousePos() const { return lastMousePos; }
void Canvas::setLastMousePos(QPointF p) { lastMousePos = p; }
