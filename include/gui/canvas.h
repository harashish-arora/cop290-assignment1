// canvas.h
/*
handles rendering and input (mouse/keyboard) for shapes.
main drawing area widget, handles mouse events and drawing shapes
also manages the list of shapes, the current selection, and the undo/redo stacks
*/
#pragma once
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWidget>
#include <memory>
#include <vector>

#include "gui/shape_mode.h"
#include "shapes/graphics_object.h"
#include "tools/canvas_state.h"
#include "tools/command.h"

class QLineEdit;  // forward declaration for text

class Canvas : public QWidget {
  Q_OBJECT

  // HistoryEntry stores a single undo/redo command
  struct HistoryEntry {
    std::unique_ptr<Command> command;  // action to undo/redo
    int prevStateId = 0;               // state id before command
    int nextStateId = 0;               // state id after command
  };

 private:
  // list of shapes currently in the document.
  // shared pointers since tools may be referenced by multiple states/commands
  // and we want automatic memory management
  std::vector<std::shared_ptr<GraphicsObject>> shapes;

  // temp shapes and currently selected shapes
  std::shared_ptr<GraphicsObject> previewShape = nullptr;
  std::shared_ptr<GraphicsObject> selectedShape = nullptr;

  // current tool selection mode like rectangle circle etc
  ShapeMode currentMode = ShapeMode::SELECT;

  QPointF startPoint;  // mouse interaction helpers
  QPointF lastMousePos;

  // states to implement mode-specific input behaviour
  std::unique_ptr<CanvasState> currentState;

  // clipboard and undo/redo stacks for commands
  std::shared_ptr<GraphicsObject> clipboard = nullptr;

  std::vector<HistoryEntry> undoStack;
  std::vector<HistoryEntry> redoStack;

  QString currentFilePath;  // path of open file (empty if unsaved)

  bool dirty = false;  // dirty flag for unsaved changes

  // ids to track saved state and current state for undo/redo
  int currentStateId = 0;
  int nextStateId = 1;

  bool historyReplayInProgress = false;

  // text helpers
  bool textEditing = false;
  QLineEdit* textEditor = nullptr;  // in-place editor widget
  std::string textBeforeEditing;    // original text before edit

  QString savedXml;  // cached xml representations for change tracking

  QString computeDocumentXml()
      const;  // compute current document XML for change tracking

  // Draft shape used while editing text (separate from previewShape).
  std::shared_ptr<GraphicsObject> textDraftShape = nullptr;

  void finalizeTextEditing();

  void syncModifiedState();

 protected:
  // handling painting and input events
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

 public:
  explicit Canvas(QWidget* parent = nullptr);
  virtual ~Canvas();

  // replace current input state
  void setState(std::unique_ptr<CanvasState> newState);

  // Accessors for shapes and selection
  std::vector<std::shared_ptr<GraphicsObject>>& getShapes();
  std::shared_ptr<GraphicsObject>& getSelectedShape();
  std::shared_ptr<GraphicsObject>& getPreviewShape();
  void setSelectedShape(std::shared_ptr<GraphicsObject> shape);
  void setPreviewShape(std::shared_ptr<GraphicsObject> shape);

  // Mode getters/setters
  ShapeMode getMode() const;
  void setMode(ShapeMode mode);

  // show/hide inline editor
  void beginTextEditing(bool selectAll = false);
  void endTextEditing();
  void setTextDraftShape(std::shared_ptr<GraphicsObject> shape);

  // mouse interaction helpers
  QPointF getStartPoint() const;
  void setStartPoint(QPointF p);
  QPointF getLastMousePos() const;
  void setLastMousePos(QPointF p);

  // push to the stacks
  void pushCommand(std::unique_ptr<Command> cmd);

  bool isModified() const;
  QString getFilePath() const;
  bool isHistoryReplayInProgress() const;

 signals:
  // emitted after selection or modification changes
  void selectionChanged();
  void modifiedChanged();

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
};
