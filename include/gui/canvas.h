#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWidget>
#include <memory>
#include <vector>

#include "shapes/graphics_object.h"
#include "tools/canvas_state.h"
#include "tools/command.h"

enum class ShapeMode {
  RECTANGLE,
  CIRCLE,
  HEXAGON,
  LINE,
  ROUNDED_RECT,
  FREEHAND,
  TEXT
};

class Canvas : public QWidget {
  // I already explained what QWidget (which inherits from QObject) is and how
  // it works in main.cpp
  // basically QWidget is like a rectangular area classthat can draw itself and
  // respond to user input, and can contain other Widgets inside it
  // btw show(), hide(), resize(w, h), move(x, y), rect(), and update()
  // update() means request a repaint and queue it, repaint() means immediate
  // repaint
 Q_OBJECT  // the QObject macro

     // what is QObject? it is basically stuff for parent-child memory,
     // and an event system

     public : explicit Canvas(QWidget* parent = nullptr);
  virtual ~Canvas() = default;

  // State transition
  void setState(std::unique_ptr<CanvasState> newState);

  // Accessors for states to read/write canvas data
  std::vector<std::shared_ptr<GraphicsObject>>& getShapes();
  std::shared_ptr<GraphicsObject>& getSelectedShape();
  std::shared_ptr<GraphicsObject>& getPreviewShape();
  void setSelectedShape(std::shared_ptr<GraphicsObject> shape);
  void setPreviewShape(std::shared_ptr<GraphicsObject> shape);

  ShapeMode getMode() const;
  void setMode(ShapeMode mode);
  void beginTextEditing();
  void endTextEditing();
  bool isTextEditing() const;
  void setTextDraftShape(std::shared_ptr<GraphicsObject> shape);

  QPointF getStartPoint() const;
  void setStartPoint(QPointF p);
  QPointF getLastMousePos() const;
  void setLastMousePos(QPointF p);

  // Clipboard for cut/copy/paste
  std::shared_ptr<GraphicsObject> getClipboard() const;
  void setClipboard(std::shared_ptr<GraphicsObject> shape);

  // Undo/redo
  void pushCommand(std::unique_ptr<Command> cmd);

  bool isModified() const;
  QString getFilePath() const;

 signals:
  void selectionChanged();
  void modifiedChanged();  // emitted when dirty state changes

 public slots:
  void copySelected();
  void cutSelected();
  void pasteAtCursor();
  void deleteSelected();
  void undo();
  void redo();
  void clearAll();
  void save();
  void saveAs();
  void openFile();
  void newFile();

 protected:
  void paintEvent(QPaintEvent* event) override;       // called for repainting
  void mousePressEvent(QMouseEvent* event) override;  // called on mouse click
  void mouseMoveEvent(QMouseEvent* event) override;   // called on mouse move
  void mouseReleaseEvent(
      QMouseEvent* event) override;  // called on mouse release
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;  // called on key press

 private:
  std::vector<std::shared_ptr<GraphicsObject>> shapes;
  std::shared_ptr<GraphicsObject> previewShape = nullptr;
  std::shared_ptr<GraphicsObject> selectedShape = nullptr;

  ShapeMode currentMode = ShapeMode::RECTANGLE;
  QPointF startPoint;
  QPointF lastMousePos;

  std::unique_ptr<CanvasState> currentState;
  std::shared_ptr<GraphicsObject> clipboard = nullptr;

  std::vector<std::unique_ptr<Command>> undoStack;
  std::vector<std::unique_ptr<Command>> redoStack;

  QString currentFilePath;  // empty until first save
  bool dirty = false;       // true when canvas has unsaved changes
  bool textEditing = false;
  std::shared_ptr<GraphicsObject> textDraftShape = nullptr;

  void finalizeTextEditing();
};
