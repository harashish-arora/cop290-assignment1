// properties_panel_helpers.cpp
// helper functions for properties panel like colour swatches, slider creation,
// and grid layout

#include "gui/properties_panel_helpers.h"

#include <QCheckBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

#include "gui/properties_panel.h"

// create configured horizontal slider used across panel rows
QSlider* makeSlider(int lo, int hi, int val) {
  auto* s = new QSlider(Qt::Horizontal);
  s->setRange(lo, hi);
  s->setValue(val);
  s->setFixedWidth(90);
  return s;
}

// create configured spinbox used across panel rows
QSpinBox* makeSpin(int lo, int hi, int val) {
  auto* s = new QSpinBox;
  s->setRange(lo, hi);
  s->setValue(val);
  s->setFixedWidth(48);
  return s;
}

// simple vertical separator widget between panel sections
QFrame* vSep() {
  auto* f = new QFrame;
  f->setFrameShape(QFrame::VLine);
  f->setFrameShadow(QFrame::Plain);
  f->setStyleSheet("color: #c0c0c0;");
  return f;
}

// stylesheet for live rgba preview button
QString previewBtnStyle(const QColor& c) {
  return QString(
             "QPushButton { background: rgba(%1,%2,%3,%4);"
             "  border: 2px solid #888; border-radius: 4px; }"
             "QPushButton:hover { border: 2px solid #333; }")
      .arg(c.red())
      .arg(c.green())
      .arg(c.blue())
      .arg(c.alpha());
}

// stylesheet for fixed preset color swatch buttons
QString swatchStyle(const QColor& c) {
  return QString(
             "QPushButton { background: %1; border: 1px solid #999;"
             "  border-radius: 3px; }"
             "QPushButton:hover { border: 2px solid #333; }")
      .arg(c.name());
}

const char* kCheckStyle =
    "QCheckBox { font-size: 10px; color: #444; font-weight: 500; }";
const char* kBoldLabel = "font-weight: 600; color: #222; font-size: 11px;";
const char* kSectionTitle = "font-weight: 700; font-size: 12px; color: #222;";

// build one colour column with title preview presets and apply on click
// checkbox
QVBoxLayout* buildColorColumn(const char* title, QPushButton*& previewOut,
                              QCheckBox*& checkOut, const QColor presets[],
                              PropertiesPanel* panel, const char* presetSlot) {
  auto* box = new QVBoxLayout;
  box->setSpacing(4);
  auto* top = new QHBoxLayout;
  top->setSpacing(6);
  auto* lbl = new QLabel(title);
  lbl->setStyleSheet(kSectionTitle);
  top->addWidget(lbl);
  top->addStretch();
  previewOut = new QPushButton;
  previewOut->setFixedSize(30, 30);
  previewOut->setCursor(Qt::PointingHandCursor);
  previewOut->setToolTip("Color picker");
  previewOut->setStyleSheet(previewBtnStyle(QColor(200, 200, 200)));
  top->addWidget(previewOut);
  box->addLayout(top);
  auto* grid = new QGridLayout;
  grid->setSpacing(4);
  for (int i = 0; i < 8; i++) {
    auto* btn = new QPushButton;
    btn->setFixedSize(22, 22);
    btn->setStyleSheet(swatchStyle(presets[i]));
    btn->setProperty("presetColor", presets[i]);
    QObject::connect(btn, SIGNAL(clicked()), panel, presetSlot);
    grid->addWidget(btn, i / 4, i % 4);
  }
  box->addLayout(grid);
  checkOut = new QCheckBox("Apply on click");
  checkOut->setStyleSheet(kCheckStyle);
  checkOut->setToolTip("Apply to clicked shapes");
  box->addWidget(checkOut);
  return box;
}
