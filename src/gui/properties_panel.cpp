// properties_panel.cpp — Constructor and color helper methods
#include "gui/properties_panel.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>

#include "gui/canvas.h"
#include "gui/properties_panel_helpers.h"

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

  // Fill Column
  mainRow->addLayout(buildColorColumn("Fill", fillPreview, fillApplyCheck,
                                      presets, this,
                                      SLOT(onFillPresetClicked())));
  connect(fillPreview, &QPushButton::clicked, this,
          &PropertiesPanel::onFillPreviewClicked);
  mainRow->addWidget(vSep());

  // Stroke Column
  mainRow->addLayout(buildColorColumn("Stroke", strokePreview, strokeApplyCheck,
                                      presets, this,
                                      SLOT(onStrokePresetClicked())));
  connect(strokePreview, &QPushButton::clicked, this,
          &PropertiesPanel::onStrokePreviewClicked);
  mainRow->addWidget(vSep());

  setupGrid(mainRow);
  connectSignals();
  setFixedHeight(120);
}

QColor PropertiesPanel::getEffectiveFill() const {
  QColor c = fillRgb_;
  c.setAlpha(fillAlphaSlider->value());
  return c;
}
QColor PropertiesPanel::getEffectiveStroke() const {
  QColor c = strokeRgb_;
  c.setAlpha(strokeAlphaSlider->value());
  return c;
}
void PropertiesPanel::updatePreviews() {
  fillPreview->setStyleSheet(previewBtnStyle(getEffectiveFill()));
  strokePreview->setStyleSheet(previewBtnStyle(getEffectiveStroke()));
}
