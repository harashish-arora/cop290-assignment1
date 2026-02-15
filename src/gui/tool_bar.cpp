// tool_bar.cpp — Shape tool selector sidebar
#include "gui/tool_bar.h"

#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <cmath>

#include "gui/canvas.h"

static QPushButton* makeToolBtn(const QSize sz,
                                std::function<void(QPainter&)> draw) {
  QPixmap pix(sz);
  pix.fill(Qt::transparent);
  QPainter p(&pix);
  p.setRenderHint(QPainter::Antialiasing);
  draw(p);
  p.end();
  auto* btn = new QPushButton;
  btn->setIcon(QIcon(pix));
  btn->setIconSize(sz);
  btn->setFixedSize(sz.width() + 8, sz.height() + 8);
  btn->setCheckable(true);
  return btn;
}

ToolBar::ToolBar(Canvas* canvas, QWidget* parent)
    : QWidget(parent), canvas(canvas) {
  auto* layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 8, 4, 4);
  layout->setSpacing(2);

  QSize iconSz(28, 28);

  rectBtn = makeToolBtn(iconSz, [](QPainter& p) {
    p.setPen(QPen(QColor("#555"), 1.5));
    p.setBrush(Qt::NoBrush);
    p.drawRect(4, 6, 20, 16);
  });

  circleBtn = makeToolBtn(iconSz, [](QPainter& p) {
    p.setPen(QPen(QColor("#555"), 1.5));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(4, 4, 20, 20);
  });

  hexBtn = makeToolBtn(iconSz, [](QPainter& p) {
    p.setPen(QPen(QColor("#555"), 1.5));
    p.setBrush(Qt::NoBrush);
    // Draw a flat-top hexagon
    QPolygonF hex;
    double cx = 14, cy = 14, r = 11;
    for (int i = 0; i < 6; i++) {
      double a = 3.14159265 / 180.0 * (60.0 * i);
      hex << QPointF(cx + r * std::cos(a), cy + r * std::sin(a));
    }
    p.drawPolygon(hex);
  });

  lineBtn = makeToolBtn(iconSz, [](QPainter& p) {
    p.setPen(QPen(QColor("#555"), 1.5));
    p.drawLine(4, 24, 24, 4);
  });

  rrBtn = makeToolBtn(iconSz, [](QPainter& p) {
    p.setPen(QPen(QColor("#555"), 1.5));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(4, 6, 20, 16, 4, 4);
  });

  freehandBtn = makeToolBtn(iconSz, [](QPainter& p) {
    p.setPen(QPen(QColor("#555"), 1.5));
    QPainterPath path;
    path.moveTo(6, 20);
    path.cubicTo(10, 6, 18, 26, 24, 10);
    p.drawPath(path);
  });

  textBtn = makeToolBtn(iconSz, [](QPainter& p) {
    p.setPen(QPen(QColor("#555"), 2));
    p.drawLine(6, 8, 22, 8);
    p.drawLine(14, 8, 14, 22);
  });

  layout->addWidget(rectBtn);
  layout->addWidget(rrBtn);
  layout->addWidget(circleBtn);
  layout->addWidget(hexBtn);
  layout->addWidget(lineBtn);
  layout->addWidget(freehandBtn);
  layout->addWidget(textBtn);
  layout->addStretch();

  rectBtn->setChecked(true);
  setFixedWidth(40);

  // Styling: left border acts as separator, subtle tool buttons
  setStyleSheet(R"(
    ToolBar {
      background: #ebebeb;
      border-left: 1px solid #c8c8c8;
    }
    QPushButton {
      background: transparent;
      border: 1px solid transparent;
      border-radius: 4px;
      padding: 3px;
    }
    QPushButton:hover {
      background: #dcdcdc;
      border: 1px solid #bbb;
    }
    QPushButton:checked {
      background: #d0d0d0;
      border: 1px solid #aaa;
    }
  )");

  connect(rectBtn, &QPushButton::clicked, this, [this]() {
    this->canvas->setMode(ShapeMode::RECTANGLE);
    updateHighlight();
  });
  connect(circleBtn, &QPushButton::clicked, this, [this]() {
    this->canvas->setMode(ShapeMode::CIRCLE);
    updateHighlight();
  });
  connect(hexBtn, &QPushButton::clicked, this, [this]() {
    this->canvas->setMode(ShapeMode::HEXAGON);
    updateHighlight();
  });
  connect(lineBtn, &QPushButton::clicked, this, [this]() {
    this->canvas->setMode(ShapeMode::LINE);
    updateHighlight();
  });
  connect(rrBtn, &QPushButton::clicked, this, [this]() {
    this->canvas->setMode(ShapeMode::ROUNDED_RECT);
    updateHighlight();
  });
  connect(freehandBtn, &QPushButton::clicked, this, [this]() {
    this->canvas->setMode(ShapeMode::FREEHAND);
    updateHighlight();
  });
  connect(textBtn, &QPushButton::clicked, this, [this]() {
    this->canvas->setMode(ShapeMode::TEXT);
    updateHighlight();
  });
}

void ToolBar::updateHighlight() {
  rectBtn->setChecked(canvas->getMode() == ShapeMode::RECTANGLE);
  rrBtn->setChecked(canvas->getMode() == ShapeMode::ROUNDED_RECT);
  circleBtn->setChecked(canvas->getMode() == ShapeMode::CIRCLE);
  hexBtn->setChecked(canvas->getMode() == ShapeMode::HEXAGON);
  lineBtn->setChecked(canvas->getMode() == ShapeMode::LINE);
  freehandBtn->setChecked(canvas->getMode() == ShapeMode::FREEHAND);
  textBtn->setChecked(canvas->getMode() == ShapeMode::TEXT);
}
