// tool_bar_icons.cpp — Icon drawing lambdas for each tool button
#include <QPainter>
#include <QPainterPath>
#include <cmath>

#include "gui/tool_bar.h"

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

void ToolBar::createButtons(const QSize& iconSz) {
  selectBtn = makeToolBtn(iconSz, [](QPainter& p) {
    p.setPen(QPen(QColor("#555"), 1.5));
    QPolygonF arrow;
    arrow << QPointF(6, 4) << QPointF(6, 22) << QPointF(11, 17)
          << QPointF(16, 24) << QPointF(18, 22) << QPointF(13, 15)
          << QPointF(19, 14);
    p.setBrush(QColor("#555"));
    p.drawPolygon(arrow);
  });
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
}
