// properties panel cpp
// fill stroke properties editor with colour picker

#include "gui/properties_panel.h"

#include <QFrame>
#include <QFontComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

#include "gui/canvas.h"
#include "gui/properties_panel_helpers.h"
#include "tools/shape_style_defaults.h"

// create panel ui sections initialize defaults and wire signals
PropertiesPanel::PropertiesPanel(Canvas* canvas, QWidget* parent)
    : QWidget(parent), canvas(canvas) {
  auto* mainRow = new QHBoxLayout(this);
  mainRow->setContentsMargins(10, 4, 10, 4);
  mainRow->setSpacing(10);

  setStyleSheet(R"(
    PropertiesPanel {
      background: #ebebeb;
      border-top: 1px solid #c0c0c0;
    }
    QLabel {
      color: #222; font-size: 11px; font-weight: 500;
    }
    QToolTip {
      background-color: #ffffff;
      color: #000000;
      border: 1px solid #aaa;
    }
  )");

  const QColor presets[] = {Qt::red,   Qt::green,  Qt::blue, Qt::black,
                            Qt::white, Qt::yellow, Qt::cyan, Qt::magenta};

  // fill column
  mainRow->addLayout(buildColorColumn("Fill", fillPreview, fillApplyCheck,
                                      presets, this,
                                      SLOT(onFillPresetClicked())));
  connect(fillPreview, &QPushButton::clicked, this,
          &PropertiesPanel::onFillPreviewClicked);
  mainRow->addWidget(vSep());

  // stroke column
  mainRow->addLayout(buildColorColumn("Stroke", strokePreview, strokeApplyCheck,
                                      presets, this,
                                      SLOT(onStrokePresetClicked())));
  connect(strokePreview, &QPushButton::clicked, this,
          &PropertiesPanel::onStrokePreviewClicked);
  mainRow->addWidget(vSep());

  setupGrid(mainRow);
  QColor defaultFill(QString::fromStdString(defaultShapeFillColor()));
  fillRgb_ = QColor(defaultFill.red(), defaultFill.green(), defaultFill.blue());
  fillAlphaSlider->setValue(defaultFill.alpha());

  QColor defaultStroke(QString::fromStdString(defaultShapeStrokeColor()));
  strokeRgb_ =
      QColor(defaultStroke.red(), defaultStroke.green(), defaultStroke.blue());
  strokeAlphaSlider->setValue(defaultStroke.alpha());
  updatePreviews();
  syncCreationDefaults();

  connectSignals();
  setFixedHeight(120);
}

// compose fill color using rgb from picker and alpha from slider
QColor PropertiesPanel::getEffectiveFill() const {
  QColor c = fillRgb_;
  c.setAlpha(fillAlphaSlider->value());
  return c;
}

// compose stroke color using rgb from picker and alpha from slider
QColor PropertiesPanel::getEffectiveStroke() const {
  QColor c = strokeRgb_;
  c.setAlpha(strokeAlphaSlider->value());
  return c;
}

// keep creation defaults synced with panel controls for new shapes
void PropertiesPanel::syncCreationDefaults() {
  auto d = getCreationDefaults();
  d.fillColor = getEffectiveFill().name(QColor::HexArgb).toStdString();
  d.strokeColor = getEffectiveStroke().name(QColor::HexArgb).toStdString();
  d.strokeWidth = widthSlider->value();
  d.cornerRadius = cornerSlider->value();
  d.hexPointyTop = pointyTopBtn->isChecked();
  d.fontFamily = fontCombo->currentFont().family().toStdString();
  d.fontSize = fontSizeSpin->value();
  setCreationDefaults(d);
}

// refresh small color preview buttons
void PropertiesPanel::updatePreviews() {
  fillPreview->setStyleSheet(previewBtnStyle(getEffectiveFill()));
  strokePreview->setStyleSheet(previewBtnStyle(getEffectiveStroke()));
}
