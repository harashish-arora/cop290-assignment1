// canvas.cpp
#include "gui/canvas.h"

#include <QAbstractButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <algorithm>
#include <cmath>
#include <fstream>

#include "parse/svg_parser.h"
#include "shapes/freehand.h"
#include "shapes/line.h"
#include "shapes/text_shape.h"
#include "tools/handle_helpers.h"
#include "tools/idle_state.h"

Canvas::Canvas(QWidget* parent) : QWidget(parent) {
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  currentState = std::make_unique<IdleState>();
  textEditing = false;
}

void Canvas::finalizeTextEditing() {
  if (!textEditing) return;

  auto txt = std::dynamic_pointer_cast<TextShape>(selectedShape);
  textEditing = false;

  if (!txt) {
    textDraftShape = nullptr;
    return;
  }

  bool isDraft = (textDraftShape && textDraftShape == selectedShape);
  if (isDraft) {
    if (txt->getText().empty()) {
      shapes.erase(std::remove(shapes.begin(), shapes.end(), selectedShape),
                   shapes.end());
      setSelectedShape(nullptr);
    } else {
      pushCommand(std::make_unique<AddShapeCommand>(selectedShape));
    }
    textDraftShape = nullptr;
  }
}

void Canvas::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.fillRect(rect(), Qt::white);
  painter.setRenderHint(QPainter::Antialiasing);

  for (const auto& shape : shapes) shape->draw(painter);

  if (previewShape) {
    QPen dash(Qt::black, 1, Qt::DashLine);
    painter.setPen(dash);
    painter.setBrush(Qt::NoBrush);
    QRectF box = previewShape->boundingBox();

    if (getMode() == ShapeMode::CIRCLE) {
      painter.drawEllipse(box);
    } else if (getMode() == ShapeMode::HEXAGON) {
      QPolygonF poly;
      double pcx = box.center().x(), pcy = box.center().y();
      double prx = box.width() / 2.0, pry = box.height() / 2.0;
      for (int i = 0; i < 6; i++) {
        double a = 3.14159265358979 / 180.0 * (60.0 * i);
        poly << QPointF(pcx + prx * std::cos(a), pcy + pry * std::sin(a));
      }
      painter.drawPolygon(poly);
    } else if (getMode() == ShapeMode::LINE) {
      auto* line = dynamic_cast<Line*>(previewShape.get());
      if (line)
        painter.drawLine(QPointF(line->getX1(), line->getY1()),
                         QPointF(line->getX2(), line->getY2()));
    } else if (getMode() == ShapeMode::FREEHAND) {
      auto* fh = dynamic_cast<Freehand*>(previewShape.get());
      if (fh) {
        const auto& pts = fh->getPoints();
        if (pts.size() >= 2) {
          QPainterPath path;
          path.moveTo(pts[0]);
          for (size_t i = 1; i < pts.size(); i++) path.lineTo(pts[i]);
          painter.drawPath(path);
        }
      }
    } else if (getMode() == ShapeMode::ROUNDED_RECT) {
      painter.drawRoundedRect(box, 10, 10);
    } else {
      painter.drawRect(box);
    }
  }

  if (selectedShape) {
    drawSelectionHandles(painter, selectedShape);
  }
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
bool Canvas::isTextEditing() const { return textEditing; }
void Canvas::setTextDraftShape(std::shared_ptr<GraphicsObject> shape) {
  textDraftShape = std::move(shape);
}
QPointF Canvas::getStartPoint() const { return startPoint; }
void Canvas::setStartPoint(QPointF p) { startPoint = p; }
QPointF Canvas::getLastMousePos() const { return lastMousePos; }
void Canvas::setLastMousePos(QPointF p) { lastMousePos = p; }

std::shared_ptr<GraphicsObject> Canvas::getClipboard() const {
  return clipboard;
}

void Canvas::setClipboard(std::shared_ptr<GraphicsObject> shape) {
  clipboard = std::move(shape);
}

void Canvas::deleteSelected() {
  if (!selectedShape) return;
  pushCommand(std::make_unique<RemoveShapeCommand>(selectedShape));
  shapes.erase(std::remove(shapes.begin(), shapes.end(), selectedShape),
               shapes.end());
  setSelectedShape(nullptr);
  update();
}

void Canvas::copySelected() {
  if (selectedShape) clipboard = selectedShape->clone();
}

void Canvas::cutSelected() {
  if (!selectedShape) return;
  clipboard = selectedShape->clone();
  deleteSelected();
}

void Canvas::pasteAtCursor() {
  if (!clipboard) return;
  auto shape = clipboard->clone();
  QRectF box = shape->boundingBox();
  shape->moveBy(lastMousePos.x() - box.center().x(),
                lastMousePos.y() - box.center().y());
  shapes.push_back(shape);
  setSelectedShape(shape);
  pushCommand(std::make_unique<AddShapeCommand>(shape));
  update();
}

void Canvas::pushCommand(std::unique_ptr<Command> cmd) {
  undoStack.push_back(std::move(cmd));
  redoStack.clear();
  if (!dirty) {
    dirty = true;
    emit modifiedChanged();
  }
}

bool Canvas::isModified() const { return dirty; }
QString Canvas::getFilePath() const { return currentFilePath; }

void Canvas::undo() {
  if (undoStack.empty()) return;
  auto cmd = std::move(undoStack.back());
  undoStack.pop_back();
  cmd->undo(this);
  redoStack.push_back(std::move(cmd));
}

void Canvas::redo() {
  if (redoStack.empty()) return;
  auto cmd = std::move(redoStack.back());
  redoStack.pop_back();
  cmd->redo(this);
  undoStack.push_back(std::move(cmd));
}

void Canvas::clearAll() {
  if (shapes.empty()) return;
  pushCommand(std::make_unique<ClearAllCommand>(shapes, selectedShape));
  shapes.clear();
  setSelectedShape(nullptr);
  update();
}

void Canvas::save() {
  if (currentFilePath.isEmpty()) {
    saveAs();
    return;
  }
  std::ofstream file(currentFilePath.toStdString());
  if (!file.is_open()) return;
  file << "<svg width=\"" << width() << "\" height=\"" << height()
       << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
  for (const auto& shape : shapes) file << "  " << shape->toSVG() << "\n";
  file << "</svg>\n";
  dirty = false;
  emit modifiedChanged();
}

void Canvas::saveAs() {
  QString path = QFileDialog::getSaveFileName(this, "Save SVG", QString(),
                                              "SVG Files (*.svg)");
  if (path.isEmpty()) return;
  currentFilePath = path;
  save();
}

void Canvas::openFile() {
  QString path = QFileDialog::getOpenFileName(this, "Open SVG", QString(),
                                              "SVG Files (*.svg)");
  if (path.isEmpty()) return;
  auto loaded = SvgParser::load(path.toStdString());
  if (loaded.empty()) return;

  shapes.clear();
  undoStack.clear();
  redoStack.clear();
  setSelectedShape(nullptr);
  shapes = std::move(loaded);
  currentFilePath = path;
  dirty = false;
  emit modifiedChanged();
  update();
}

void Canvas::newFile() {
  if (dirty) {
    QMessageBox box(this);
    box.setIcon(QMessageBox::NoIcon);
    box.setWindowTitle("Unsaved Changes");
    box.setText("You have unsaved changes. Save before creating a new file?");
    QPushButton* saveBtn = box.addButton("Save", QMessageBox::AcceptRole);
    box.addButton("Don't Save", QMessageBox::DestructiveRole);
    QPushButton* cancelBtn = box.addButton("Cancel", QMessageBox::RejectRole);
    box.setDefaultButton(cancelBtn);
    box.exec();
    QAbstractButton* clicked = box.clickedButton();
    if (!clicked || clicked == cancelBtn) return;
    if (clicked == saveBtn) save();
  }

  shapes.clear();
  undoStack.clear();
  redoStack.clear();
  setSelectedShape(nullptr);
  previewShape = nullptr;
  clipboard = nullptr;
  currentFilePath.clear();
  dirty = false;
  emit modifiedChanged();
  update();
}
