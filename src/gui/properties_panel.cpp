// properties_panel.cpp — Horizontal color/stroke properties editor
#include "gui/properties_panel.h"

#include <QFontComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

#include "gui/canvas.h"
#include "shapes/hexagon.h"
#include "shapes/rounded_rectangle.h"
#include "shapes/text_shape.h"

static QSlider* makeSlider(int lo, int hi, int val) {
  auto* s = new QSlider(Qt::Horizontal);
  s->setRange(lo, hi);
  s->setValue(val);
  s->setFixedWidth(90);
  return s;
}
static QSpinBox* makeSpin(int lo, int hi, int val) {
  auto* s = new QSpinBox;
  s->setRange(lo, hi);
  s->setValue(val);
  s->setFixedWidth(48);
  return s;
}
static QFrame* vSep() {
  auto* f = new QFrame;
  f->setFrameShape(QFrame::VLine);
  f->setFrameShadow(QFrame::Plain);
  f->setStyleSheet("color: #c0c0c0;");
  return f;
}

PropertiesPanel::PropertiesPanel(Canvas* canvas, QWidget* parent)
    : QWidget(parent), canvas(canvas) {
  auto* row = new QHBoxLayout(this);
  row->setContentsMargins(10, 6, 10, 6);
  row->setSpacing(12);

  // Panel-level styling
  setStyleSheet(R"(
    PropertiesPanel {
      background: #ebebeb;
      border-top: 1px solid #c0c0c0;
    }
    QLabel {
      color: #222;
      font-size: 11px;
      font-weight: 500;
    }
  )");

  // -- 1. Fill / Stroke toggle --
  auto* modeBox = new QVBoxLayout;
  modeBox->setSpacing(4);
  fillBtn = new QPushButton("Fill");
  strokeBtn = new QPushButton("Stroke");
  for (auto* b : {fillBtn, strokeBtn}) {
    b->setCheckable(true);
    b->setFixedSize(56, 24);
    b->setStyleSheet(R"(
      QPushButton {
        background: #ddd;
        border: 1px solid #aaa;
        border-radius: 4px;
        font-size: 11px;
        font-weight: 600;
        color: #333;
      }
      QPushButton:hover { background: #d0d0d0; }
      QPushButton:checked {
        background: #0078d4;
        color: white;
        border: 1px solid #005a9e;
      }
    )");
  }
  fillBtn->setChecked(true);
  modeBox->addWidget(fillBtn);
  modeBox->addWidget(strokeBtn);
  row->addLayout(modeBox);
  row->addWidget(vSep());

  // -- 2. 8 preset colors (4x2 grid) --
  auto* grid = new QGridLayout;
  grid->setSpacing(3);
  const QColor presets[] = {Qt::red,   Qt::green,  Qt::blue, Qt::black,
                            Qt::white, Qt::yellow, Qt::cyan, Qt::magenta};
  for (int i = 0; i < 8; i++) {
    auto* btn = new QPushButton;
    btn->setFixedSize(20, 20);
    QString bg = presets[i].name();
    // Ensure white and light colors have visible borders
    btn->setStyleSheet(QString("QPushButton { background: %1; border: 1px "
                               "solid #888; border-radius: 2px; }"
                               "QPushButton:hover { border: 2px solid #444; }")
                           .arg(bg));
    btn->setProperty("presetColor", presets[i]);
    connect(btn, &QPushButton::clicked, this,
            &PropertiesPanel::onPresetClicked);
    grid->addWidget(btn, i / 4, i % 4);
  }
  row->addLayout(grid);
  row->addWidget(vSep());

  // -- 3. RGBAW sliders (W is 5th row, shown only in stroke mode) --
  sliderGrid = new QGridLayout;
  sliderGrid->setSpacing(2);
  sliderGrid->setColumnMinimumWidth(0, 14);  // label column

  const char* names[] = {"R", "G", "B", "A", "W"};
  QSlider* sliders[5];
  QSpinBox* spins[5];
  for (int i = 0; i < 5; i++) {
    auto* label = new QLabel(names[i]);
    label->setStyleSheet("font-weight: bold; color: #222;");
    sliderGrid->addWidget(label, i, 0);
    if (i < 4) {
      sliders[i] = makeSlider(0, 255, i == 3 ? 255 : 0);
      spins[i] = makeSpin(0, 255, i == 3 ? 255 : 0);
    } else {
      sliders[i] = makeSlider(1, 20, 1);
      spins[i] = makeSpin(1, 20, 1);
    }
    sliderGrid->addWidget(sliders[i], i, 1);
    sliderGrid->addWidget(spins[i], i, 2);
    connect(sliders[i], &QSlider::valueChanged, spins[i], &QSpinBox::setValue);
    connect(spins[i], QOverload<int>::of(&QSpinBox::valueChanged), sliders[i],
            &QSlider::setValue);
    connect(sliders[i], &QSlider::valueChanged, this,
            &PropertiesPanel::applyToShape);
  }
  rSlider = sliders[0];
  gSlider = sliders[1];
  bSlider = sliders[2];
  alphaSlider = sliders[3];
  widthSlider = sliders[4];
  rSpin = spins[0];
  gSpin = spins[1];
  bSpin = spins[2];
  alphaSpin = spins[3];
  widthSpin = spins[4];

  // Store the W row label so we can show/hide it
  widthLabel =
      qobject_cast<QLabel*>(sliderGrid->itemAtPosition(4, 0)->widget());

  // Initially hide W row (fill mode)
  widthLabel->hide();
  widthSlider->hide();
  widthSpin->hide();

  // Corner radius row (6th row, visible only for RoundedRectangle)
  // (moved to shape-specific section on the right)

  row->addLayout(sliderGrid);
  row->addWidget(vSep());

  // -- 4. Preview swatch with label --
  auto* previewBox = new QVBoxLayout;
  previewBox->setSpacing(2);
  previewBox->setAlignment(Qt::AlignCenter);

  auto* previewLabel = new QLabel("Preview");
  previewLabel->setAlignment(Qt::AlignCenter);
  previewLabel->setStyleSheet(
      "font-size: 10px; color: #555; font-weight: 500;");
  previewBox->addWidget(previewLabel);

  preview = new QWidget;
  preview->setFixedSize(36, 36);
  preview->setAutoFillBackground(true);
  preview->setStyleSheet(
      "background: white; border: 1px solid #999; border-radius: 4px;");
  previewBox->addWidget(preview, 0, Qt::AlignCenter);

  row->addLayout(previewBox);
  row->addStretch();

  // -- 5. Shape-specific section (right side) --
  shapeSection = new QWidget;
  auto* shapeLayout = new QVBoxLayout(shapeSection);
  shapeLayout->setContentsMargins(0, 0, 0, 0);
  shapeLayout->setSpacing(4);

  // — Corner Radius (RoundedRectangle) —
  auto* crRow = new QHBoxLayout;
  crRow->setSpacing(4);
  cornerLabel = new QLabel("Radius");
  cornerLabel->setStyleSheet("font-weight: 600; color: #222; font-size: 11px;");
  cornerSlider = makeSlider(0, 100, 10);
  cornerSlider->setFixedWidth(70);
  cornerSpin = makeSpin(0, 100, 10);
  crRow->addWidget(cornerLabel);
  crRow->addWidget(cornerSlider);
  crRow->addWidget(cornerSpin);
  shapeLayout->addLayout(crRow);
  connect(cornerSlider, &QSlider::valueChanged, cornerSpin,
          &QSpinBox::setValue);
  connect(cornerSpin, QOverload<int>::of(&QSpinBox::valueChanged), cornerSlider,
          &QSlider::setValue);
  connect(cornerSlider, &QSlider::valueChanged, this,
          &PropertiesPanel::applyToShape);

  // — Orientation (Hexagon) —
  auto* oriRow = new QHBoxLayout;
  oriRow->setSpacing(4);
  auto* oriLabel = new QLabel("Orientation");
  oriLabel->setStyleSheet("font-weight: 600; color: #222; font-size: 11px;");
  flatTopBtn = new QPushButton("Flat");
  pointyTopBtn = new QPushButton("Pointy");
  for (auto* b : {flatTopBtn, pointyTopBtn}) {
    b->setCheckable(true);
    b->setFixedSize(48, 22);
    b->setStyleSheet(R"(
      QPushButton {
        background: #ddd;
        border: 1px solid #aaa;
        border-radius: 3px;
        font-size: 10px;
        font-weight: 600;
        color: #333;
      }
      QPushButton:hover { background: #d0d0d0; }
      QPushButton:checked {
        background: #0078d4;
        color: white;
        border: 1px solid #005a9e;
      }
    )");
  }
  flatTopBtn->setChecked(true);
  oriRow->addWidget(oriLabel);
  oriRow->addWidget(flatTopBtn);
  oriRow->addWidget(pointyTopBtn);
  shapeLayout->addLayout(oriRow);
  connect(flatTopBtn, &QPushButton::clicked, this, [this]() {
    flatTopBtn->setChecked(true);
    pointyTopBtn->setChecked(false);
    applyToShape();
  });
  connect(pointyTopBtn, &QPushButton::clicked, this, [this]() {
    pointyTopBtn->setChecked(true);
    flatTopBtn->setChecked(false);
    applyToShape();
  });

  // — Text controls —
  auto* textRow = new QHBoxLayout;
  textRow->setSpacing(4);
  fontLabel = new QLabel("Font");
  fontLabel->setStyleSheet("font-weight: 600; color: #222; font-size: 11px;");
  fontCombo = new QFontComboBox;
  fontCombo->setFixedWidth(110);
  fontCombo->setFontFilters(QFontComboBox::ScalableFonts);
  fontCombo->setStyleSheet(R"(
    QFontComboBox {
      background: white;
      color: #222;
      border: 1px solid #bbb;
      border-radius: 3px;
      padding: 1px 4px;
      font-size: 11px;
    }
    QFontComboBox QAbstractItemView {
      background: white;
      color: #222;
      selection-background-color: #dceeff;
      selection-color: #111;
    }
  )");
  fontSizeLabel = new QLabel("Size");
  fontSizeLabel->setStyleSheet(
      "font-weight: 600; color: #222; font-size: 11px;");
  fontSizeSpin = makeSpin(6, 200, 16);
  fontSizeSpin->setStyleSheet(
      "QSpinBox { background: white; color: #222; border: 1px solid #bbb; "
      "border-radius: 3px; padding: 1px 2px; font-size: 11px; }");
  textRow->addWidget(fontLabel);
  textRow->addWidget(fontCombo);
  textRow->addWidget(fontSizeLabel);
  textRow->addWidget(fontSizeSpin);
  shapeLayout->addLayout(textRow);
  connect(fontCombo, &QFontComboBox::currentFontChanged, this,
          [this](const QFont&) { applyToShape(); });
  connect(fontSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int) { applyToShape(); });

  shapeSection->hide();
  row->addWidget(shapeSection);

  // -- Toggle connections --
  connect(fillBtn, &QPushButton::clicked, this, [this]() {
    fillBtn->setChecked(true);
    strokeBtn->setChecked(false);
    widthLabel->hide();
    widthSlider->hide();
    widthSpin->hide();
    refreshFromSelection();
  });
  connect(strokeBtn, &QPushButton::clicked, this, [this]() {
    strokeBtn->setChecked(true);
    fillBtn->setChecked(false);
    widthLabel->show();
    widthSlider->show();
    widthSpin->show();
    refreshFromSelection();
  });
  setFixedHeight(120);
}

bool PropertiesPanel::isStrokeMode() const { return strokeBtn->isChecked(); }
QColor PropertiesPanel::currentColor() const {
  return {rSlider->value(), gSlider->value(), bSlider->value(),
          alphaSlider->value()};
}
void PropertiesPanel::updateDisplay() {
  auto p = preview->palette();
  p.setColor(QPalette::Window, currentColor());
  preview->setPalette(p);
}
void PropertiesPanel::refreshFromSelection() {
  updating = true;
  auto shape = canvas->getSelectedShape();
  if (!shape) {
    rSlider->setValue(0);
    gSlider->setValue(0);
    bSlider->setValue(0);
    alphaSlider->setValue(255);
    widthSlider->setValue(1);
    shapeSection->hide();
  } else {
    std::string cs =
        isStrokeMode() ? shape->getStrokeColor() : shape->getFillColor();
    QColor c(QString::fromStdString(cs));
    rSlider->setValue(c.red());
    gSlider->setValue(c.green());
    bSlider->setValue(c.blue());
    alphaSlider->setValue(c.alpha());
    widthSlider->setValue(static_cast<int>(shape->getStrokeWidth()));

    // Show/hide shape-specific controls
    auto rr = std::dynamic_pointer_cast<RoundedRectangle>(shape);
    auto hex = std::dynamic_pointer_cast<Hexagon>(shape);
    auto text = std::dynamic_pointer_cast<TextShape>(shape);
    bool showSection = false;

    // Corner radius (rounded rect)
    cornerLabel->setVisible(!!rr);
    cornerSlider->setVisible(!!rr);
    cornerSpin->setVisible(!!rr);
    if (rr) {
      cornerSlider->setValue(static_cast<int>(rr->getCornerRadius()));
      showSection = true;
    }

    // Orientation (hexagon)
    flatTopBtn->parentWidget();  // ensure children accessible
    bool isHex = !!hex;
    // Show/hide orientation row widgets
    auto* oriLayout =
        qobject_cast<QHBoxLayout*>(shapeSection->layout()->itemAt(1)->layout());
    if (oriLayout) {
      for (int i = 0; i < oriLayout->count(); i++)
        if (auto* w = oriLayout->itemAt(i)->widget()) w->setVisible(isHex);
    }
    if (hex) {
      flatTopBtn->setChecked(!hex->isPointyTop());
      pointyTopBtn->setChecked(hex->isPointyTop());
      showSection = true;
    }

    // Text (font + size)
    bool isText = !!text;
    fontLabel->setVisible(isText);
    fontCombo->setVisible(isText);
    fontSizeLabel->setVisible(isText);
    fontSizeSpin->setVisible(isText);
    if (text) {
      fontCombo->setCurrentFont(
          QFont(QString::fromStdString(text->getFontFamily())));
      fontSizeSpin->setValue(text->getFontSize());
      showSection = true;
    }

    shapeSection->setVisible(showSection);
  }
  updateDisplay();
  updating = false;
}
void PropertiesPanel::applyToShape() {
  if (updating) return;
  auto shape = canvas->getSelectedShape();
  if (!shape) return;
  QColor c = currentColor();
  std::string hex = c.name(QColor::HexArgb).toStdString();
  if (isStrokeMode()) {
    shape->setStrokeColor(hex);
    shape->setStrokeWidth(widthSlider->value());
  } else {
    shape->setFillColor(hex);
  }
  auto rr = std::dynamic_pointer_cast<RoundedRectangle>(shape);
  if (rr) rr->setCornerRadius(cornerSlider->value());
  auto hexShape = std::dynamic_pointer_cast<Hexagon>(shape);
  if (hexShape) hexShape->setPointyTop(pointyTopBtn->isChecked());
  auto text = std::dynamic_pointer_cast<TextShape>(shape);
  if (text) {
    text->setFontFamily(fontCombo->currentFont().family().toStdString());
    text->setFontSize(fontSizeSpin->value());
  }
  updateDisplay();
  canvas->update();
}
void PropertiesPanel::onPresetClicked() {
  auto* btn = qobject_cast<QPushButton*>(sender());
  if (!btn) return;
  QColor c = btn->property("presetColor").value<QColor>();
  updating = true;
  rSlider->setValue(c.red());
  gSlider->setValue(c.green());
  bSlider->setValue(c.blue());
  updating = false;
  applyToShape();
}
