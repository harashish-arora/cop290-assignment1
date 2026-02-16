// tool_bar.h — Shape tool selector sidebar
#pragma once
#include <QPushButton>
#include <QWidget>

class Canvas;

class ToolBar : public QWidget {
  Q_OBJECT
 public:
  explicit ToolBar(Canvas* canvas, QWidget* parent = nullptr);

 private:
  Canvas* canvas;
  QPushButton* selectBtn;
  QPushButton* rectBtn;
  QPushButton* circleBtn;
  QPushButton* hexBtn;
  QPushButton* lineBtn;
  QPushButton* rrBtn;
  QPushButton* freehandBtn;
  QPushButton* textBtn;
  void updateHighlight();
  void createButtons(const QSize& iconSz);
};
