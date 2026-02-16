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
class QLineEdit;

class Canvas : public QWidget {
  Q_OBJECT
  struct HistoryEntry {
    std::unique_ptr<Command> command;
    int prevStateId = 0;
    int nextStateId = 0;
  };
 public:
  explicit Canvas(QWidget* parent = nullptr);
  virtual ~Canvas() = default;
  void setState(std::unique_ptr<CanvasState> newState);
  std::vector<std::shared_ptr<GraphicsObject>>& getShapes();
  std::shared_ptr<GraphicsObject>& getSelectedShape();
  std::shared_ptr<GraphicsObject>& getPreviewShape();
  void setSelectedShape(std::shared_ptr<GraphicsObject> shape);
  void setPreviewShape(std::shared_ptr<GraphicsObject> shape);
  ShapeMode getMode() const;
  void setMode(ShapeMode mode);
  void beginTextEditing(bool selectAll = false);
  void endTextEditing();
  void setTextDraftShape(std::shared_ptr<GraphicsObject> shape);
  QPointF getStartPoint() const;
  void setStartPoint(QPointF p);
  QPointF getLastMousePos() const;
  void setLastMousePos(QPointF p);
  void pushCommand(std::unique_ptr<Command> cmd);
  bool isModified() const;
  QString getFilePath() const;
  bool isHistoryReplayInProgress() const;
 signals:
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
 protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;
 private:
  std::vector<std::shared_ptr<GraphicsObject>> shapes;
  std::shared_ptr<GraphicsObject> previewShape = nullptr;
  std::shared_ptr<GraphicsObject> selectedShape = nullptr;
  ShapeMode currentMode = ShapeMode::SELECT;
  QPointF startPoint;
  QPointF lastMousePos;
  std::unique_ptr<CanvasState> currentState;
  std::shared_ptr<GraphicsObject> clipboard = nullptr;
  std::vector<HistoryEntry> undoStack;
  std::vector<HistoryEntry> redoStack;
  QString currentFilePath;
  bool dirty = false;
  int currentStateId = 0;
  int savedStateId = 0;
  int nextStateId = 1;
  bool historyReplayInProgress = false;
  bool textEditing = false;
  QLineEdit* textEditor = nullptr;
  std::string textBeforeEditing;
  QString savedXml;
  QString computeDocumentXml() const;
  std::shared_ptr<GraphicsObject> textDraftShape = nullptr;
  void finalizeTextEditing();
  void syncModifiedState();
};
