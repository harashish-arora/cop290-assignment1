// tool bar cpp
// minimal toolbar that hosts tool buttons select, rectangle, circle

#include "gui/tool_bar.h"

#include <QVBoxLayout>

#include "gui/canvas.h"

// build toolbar layout styles and signal bindings for shape modes
ToolBar::ToolBar(Canvas* canvas, QWidget* parent)
    : QWidget(parent), canvas(canvas) {
  auto* layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 8, 4, 4);
  layout->setSpacing(2);

  createButtons(QSize(28, 28));

  selectBtn->setToolTip("Select");
  rectBtn->setToolTip("Rectangle");
  rrBtn->setToolTip("Rounded Rectangle");
  circleBtn->setToolTip("Circle");
  hexBtn->setToolTip("Hexagon");
  lineBtn->setToolTip("Line");
  freehandBtn->setToolTip("Freehand");
  textBtn->setToolTip("Text");

  layout->addWidget(selectBtn);
  layout->addWidget(rectBtn);
  layout->addWidget(rrBtn);
  layout->addWidget(circleBtn);
  layout->addWidget(hexBtn);
  layout->addWidget(lineBtn);
  layout->addWidget(freehandBtn);
  layout->addWidget(textBtn);
  layout->addStretch();

  selectBtn->setChecked(true);
  setFixedWidth(40);

  setStyleSheet(R"(
    ToolBar {
      background: #ebebeb;
      border-left: 1px solid #c8c8c8;
    }
    QPushButton {
      background: transparent;
      border: 1px solid transparent;
      border-radius: 4px; padding: 3px;
    }
    QPushButton:hover {
      background: #dcdcdc; border: 1px solid #bbb;
    }
    QPushButton:checked {
      background: #d0d0d0; border: 1px solid #aaa;
    }
    QToolTip {
      background-color: #ffffff;
      color: #000000; border: 1px solid #aaa;
    }
  )");

  // each button sets canvas mode then updates visual highlight
  auto bind = [&](QPushButton* btn, ShapeMode mode) {
    connect(btn, &QPushButton::clicked, this, [this, mode]() {
      this->canvas->setMode(mode);
      updateHighlight();
    });
  };
  bind(selectBtn, ShapeMode::SELECT);
  bind(rectBtn, ShapeMode::RECTANGLE);
  bind(circleBtn, ShapeMode::CIRCLE);
  bind(hexBtn, ShapeMode::HEXAGON);
  bind(lineBtn, ShapeMode::LINE);
  bind(rrBtn, ShapeMode::ROUNDED_RECT);
  bind(freehandBtn, ShapeMode::FREEHAND);
  bind(textBtn, ShapeMode::TEXT);
}

// keep checked state synced with current canvas mode
void ToolBar::updateHighlight() {
  selectBtn->setChecked(canvas->getMode() == ShapeMode::SELECT);
  rectBtn->setChecked(canvas->getMode() == ShapeMode::RECTANGLE);
  rrBtn->setChecked(canvas->getMode() == ShapeMode::ROUNDED_RECT);
  circleBtn->setChecked(canvas->getMode() == ShapeMode::CIRCLE);
  hexBtn->setChecked(canvas->getMode() == ShapeMode::HEXAGON);
  lineBtn->setChecked(canvas->getMode() == ShapeMode::LINE);
  freehandBtn->setChecked(canvas->getMode() == ShapeMode::FREEHAND);
  textBtn->setChecked(canvas->getMode() == ShapeMode::TEXT);
}
