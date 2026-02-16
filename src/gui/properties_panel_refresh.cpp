// properties panel refresh cpp
// refresh controls from currently selected shape

#include <QCheckBox>
#include <QFontComboBox>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

#include "gui/canvas.h"
#include "gui/properties_panel.h"
#include "gui/properties_panel_helpers.h"

// pull selected shape state into ui controls
// optional apply on click can push changes back to shape on selection change
void PropertiesPanel::refreshFromSelection() {
  updating = true;
  auto shape = canvas->getSelectedShape();
  if (sliderInteractionActive && sliderInteractionShape != shape) {
    sliderInteractionActive = false;
    sliderInteractionShape = nullptr;
  }

  if (shape) {
    bool allowAutoApply = !canvas->isHistoryReplayInProgress();
    if (allowAutoApply) {
      // apply panel values immediately when apply on click is enabled
      auto before = captureShapeState(shape);
      auto after = before;
      if (fillApplyCheck->isChecked())
        after.fillColor = getEffectiveFill().name(QColor::HexArgb).toStdString();
      if (strokeApplyCheck->isChecked()) {
        after.strokeColor =
            getEffectiveStroke().name(QColor::HexArgb).toStdString();
        after.strokeWidth = widthSlider->value();
      }
      if (after != before) {
        applyStateToShape(shape, after);
        pushShapeStateCommand(shape, before, after);
        canvas->update();
      }
    }

    // now mirror selected shape values into panel widgets
    auto state = captureShapeState(shape);

    QColor fc(QString::fromStdString(state.fillColor));
    fillRgb_ = QColor(fc.red(), fc.green(), fc.blue());
    fillAlphaSlider->setValue(fc.alpha());

    QColor sc(QString::fromStdString(state.strokeColor));
    strokeRgb_ = QColor(sc.red(), sc.green(), sc.blue());
    strokeAlphaSlider->setValue(sc.alpha());
    widthSlider->setValue(static_cast<int>(state.strokeWidth));

    if (state.hasCornerRadius)
      cornerSlider->setValue(static_cast<int>(state.cornerRadius));
    if (state.hasPointyTop) {
      flatTopBtn->setChecked(!state.pointyTop);
      pointyTopBtn->setChecked(state.pointyTop);
    }
    if (state.hasTextStyle) {
      fontCombo->setCurrentFont(QFont(QString::fromStdString(state.fontFamily)));
      fontSizeSpin->setValue(state.fontSize);
    }
  }

  updatePreviews();
  syncCreationDefaults();
  updating = false;
}

// apply current panel controls to selected shape and record undo command
void PropertiesPanel::applyToShape() {
  if (updating) return;
  updatePreviews();
  syncCreationDefaults();
  auto shape = canvas->getSelectedShape();
  if (!shape) return;

  auto before = captureShapeState(shape);
  auto after = before;
  after.fillColor = getEffectiveFill().name(QColor::HexArgb).toStdString();
  after.strokeColor = getEffectiveStroke().name(QColor::HexArgb).toStdString();
  after.strokeWidth = widthSlider->value();
  if (after.hasCornerRadius) after.cornerRadius = cornerSlider->value();
  if (after.hasPointyTop) after.pointyTop = pointyTopBtn->isChecked();
  if (after.hasTextStyle) {
    after.fontFamily = fontCombo->currentFont().family().toStdString();
    after.fontSize = fontSizeSpin->value();
  }

  if (before == after) {
    canvas->update();
    return;
  }

  applyStateToShape(shape, after);
  if (!sliderInteractionActive) pushShapeStateCommand(shape, before, after);

  updatePreviews();
  canvas->update();
}
