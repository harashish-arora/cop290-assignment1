// tool_bar.h — compact toolbar for choosing drawing tools
#pragma once
#include <QPushButton>
#include <QWidget>

class Canvas;

// Minimal toolbar that hosts tool buttons (select, rectangle, circle...)
// constructed with a pointer to Canvas this toolbar controls
class ToolBar : public QWidget {
  Q_OBJECT

 private:
  Canvas* canvas;  // non-owning pointer to Canvas
  QPushButton* selectBtn;
  QPushButton* rectBtn;
  QPushButton* circleBtn;
  QPushButton* hexBtn;
  QPushButton* lineBtn;
  QPushButton* rrBtn;  // rounded-rect
  QPushButton* freehandBtn;
  QPushButton* textBtn;
  void updateHighlight();  // update which button appears active
  void createButtons(const QSize& iconSz);  // helper to create buttons

 public:
  explicit ToolBar(Canvas* canvas,
                   QWidget* parent = nullptr);  // interface setup
};
