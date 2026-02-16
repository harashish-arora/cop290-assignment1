// canvas.cpp — constructor and mouse event dispatch
#include "gui/canvas.h"

#include <QLineEdit>
#include "tools/idle_state.h"

Canvas::Canvas(QWidget* parent) : QWidget(parent) {
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  currentState = std::make_unique<IdleState>();
  textEditing = false;

  textEditor = new QLineEdit(this);
  textEditor->hide();
  textEditor->setFrame(true);
  textEditor->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  // Keep typing color black; use a light semi-transparent blue for selection
  textEditor->setStyleSheet(
      "color: black;"
      "selection-background-color: rgba(100,150,255,140);"
      "selection-color: black;"
      "background: rgba(0,0,0,20);");

  connect(textEditor, &QLineEdit::returnPressed, this,
          [this]() { finalizeTextEditing(); });
  connect(textEditor, &QLineEdit::editingFinished, this,
          [this]() { finalizeTextEditing(); });
}

void Canvas::mousePressEvent(QMouseEvent* e) {
  if (textEditing) finalizeTextEditing();
  currentState->handleMousePress(this, e);
}
void Canvas::mouseMoveEvent(QMouseEvent* e) {
  currentState->handleMouseMove(this, e);
}
void Canvas::mouseReleaseEvent(QMouseEvent* e) {
  currentState->handleMouseRelease(this, e);
}
