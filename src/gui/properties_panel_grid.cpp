// properties_panel_grid.cpp
// build grid section with sliders toggles and text controls

#include <QFontComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

#include "gui/properties_panel.h"
#include "gui/properties_panel_helpers.h"

void PropertiesPanel::setupGrid(QHBoxLayout* mainRow) {
  auto* grid = new QGridLayout;
  grid->setSpacing(4);
  grid->setColumnMinimumWidth(3, 12);
  {
    // row one: fill opacity and stroke width
    auto* l = new QLabel("Fill Opacity");
    l->setStyleSheet(kBoldLabel);
    l->setToolTip("Fill transparency (0-255)");
    grid->addWidget(l, 0, 0);
    fillAlphaSlider = makeSlider(0, 255, 255);
    fillAlphaSpin = makeSpin(0, 255, 255);
    grid->addWidget(fillAlphaSlider, 0, 1);
    grid->addWidget(fillAlphaSpin, 0, 2);
    auto* wl = new QLabel("Stroke Width");
    wl->setStyleSheet(kBoldLabel);
    wl->setToolTip("Border width (1-20)");
    grid->addWidget(wl, 0, 4);
    widthSlider = makeSlider(1, 20, 1);
    widthSpin = makeSpin(1, 20, 1);
    grid->addWidget(widthSlider, 0, 5);
    grid->addWidget(widthSpin, 0, 6);
  }
  {
    // row two: stroke opacity and rounded rectangle corner radius
    auto* l = new QLabel("Stroke Opacity");
    l->setStyleSheet(kBoldLabel);
    l->setToolTip("Stroke transparency (0-255)");
    grid->addWidget(l, 1, 0);
    strokeAlphaSlider = makeSlider(0, 255, 255);
    strokeAlphaSpin = makeSpin(0, 255, 255);
    grid->addWidget(strokeAlphaSlider, 1, 1);
    grid->addWidget(strokeAlphaSpin, 1, 2);
    auto* cl = new QLabel("RR Corner Radius");
    cl->setStyleSheet(kBoldLabel);
    cl->setToolTip("Rounded Rectangle corners");
    grid->addWidget(cl, 1, 4);
    cornerSlider = makeSlider(0, 100, 10);
    cornerSpin = makeSpin(0, 100, 10);
    grid->addWidget(cornerSlider, 1, 5);
    grid->addWidget(cornerSpin, 1, 6);
  }
  {
    // row three: hex orientation toggle buttons
    auto* oriBox = new QHBoxLayout;
    oriBox->setSpacing(4);
    auto* ol = new QLabel("Hex Orientation");
    ol->setStyleSheet(kBoldLabel);
    ol->setToolTip("Hexagon orientation");
    flatTopBtn = new QPushButton("Flat");
    pointyTopBtn = new QPushButton("Pointy");
    for (auto* b : {flatTopBtn, pointyTopBtn}) {
      b->setCheckable(true);
      b->setFixedSize(48, 22);
      b->setStyleSheet(
          "QPushButton{background:#ddd;border:1px solid #aaa;"
          "border-radius:3px;font-size:10px;font-weight:600;color:#333}"
          "QPushButton:hover{background:#d0d0d0}"
          "QPushButton:checked{background:#0078d4;color:white;"
          "border:1px solid #005a9e}");
    }
    flatTopBtn->setChecked(true);
    oriBox->addWidget(ol);
    oriBox->addWidget(flatTopBtn);
    oriBox->addWidget(pointyTopBtn);
    grid->addLayout(oriBox, 2, 0, 1, 3);

    // row three: text font controls
    auto* fontBox = new QHBoxLayout;
    fontBox->setSpacing(4);
    auto* fl = new QLabel("Text Font");
    fl->setStyleSheet(kBoldLabel);
    fl->setToolTip("Font for Text shapes");
    fontCombo = new QFontComboBox;
    fontCombo->setFixedWidth(120);
    fontCombo->setFontFilters(QFontComboBox::AllFonts);
    fontCombo->setCurrentFont(QFont("Arial"));
    fontCombo->setStyleSheet(
        "QFontComboBox{background:white;color:#222;"
        "border:1px solid #bbb;border-radius:3px;"
        "padding:1px 4px;font-size:11px}");
    auto* sl = new QLabel("Size");
    sl->setStyleSheet(kBoldLabel);
    fontSizeSpin = makeSpin(6, 200, 16);
    fontBox->addWidget(fl);
    fontBox->addWidget(fontCombo);
    fontBox->addWidget(sl);
    fontBox->addWidget(fontSizeSpin);
    grid->addLayout(fontBox, 2, 4, 1, 3);
  }

  mainRow->addLayout(grid);
  mainRow->addStretch();
}
