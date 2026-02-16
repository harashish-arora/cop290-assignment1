// properties_panel_refresh.cpp — Read shape state into panel, apply panel to
// shape
#include <QCheckBox>
#include <QFontComboBox>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

#include "gui/canvas.h"
#include "gui/properties_panel.h"
#include "gui/properties_panel_helpers.h"
#include "shapes/hexagon.h"
#include "shapes/rounded_rectangle.h"
#include "shapes/text_shape.h"

ShapePropertyState PropertiesPanel::captureShapeState(
    const std::shared_ptr<GraphicsObject>& shape) const {
  ShapePropertyState state;
  if (!shape) return state;

  state.fillColor = shape->getFillColor();
  state.strokeColor = shape->getStrokeColor();
  state.strokeWidth = shape->getStrokeWidth();

  if (auto rr = std::dynamic_pointer_cast<RoundedRectangle>(shape)) {
    state.hasCornerRadius = true;
    state.cornerRadius = rr->getCornerRadius();
  }

  if (auto hex = std::dynamic_pointer_cast<Hexagon>(shape)) {
    state.hasPointyTop = true;
    state.pointyTop = hex->isPointyTop();
  }

  if (auto text = std::dynamic_pointer_cast<TextShape>(shape)) {
    state.hasTextStyle = true;
    state.fontFamily = text->getFontFamily();
    state.fontSize = text->getFontSize();
    state.hasTextContent = true;
    state.textContent = text->getText();
  }

  return state;
}

void PropertiesPanel::applyStateToShape(
    const std::shared_ptr<GraphicsObject>& shape,
    const ShapePropertyState& state) const {
  if (!shape) return;

  shape->setFillColor(state.fillColor);
  shape->setStrokeColor(state.strokeColor);
  shape->setStrokeWidth(state.strokeWidth);

  if (state.hasCornerRadius) {
    if (auto rr = std::dynamic_pointer_cast<RoundedRectangle>(shape))
      rr->setCornerRadius(state.cornerRadius);
  }

  if (state.hasPointyTop) {
    if (auto hex = std::dynamic_pointer_cast<Hexagon>(shape))
      hex->setPointyTop(state.pointyTop);
  }

  if (state.hasTextStyle) {
    if (auto text = std::dynamic_pointer_cast<TextShape>(shape)) {
      text->setFontFamily(state.fontFamily);
      text->setFontSize(state.fontSize);
    }
  }
}

void PropertiesPanel::pushShapeStateCommand(
    const std::shared_ptr<GraphicsObject>& shape,
    const ShapePropertyState& before,
    const ShapePropertyState& after) {
  if (!shape || before == after) return;
  canvas->pushCommand(
      std::make_unique<ShapePropertyCommand>(shape, before, after));
}

void PropertiesPanel::beginSliderInteraction() {
  if (updating || sliderInteractionActive) return;
  auto shape = canvas->getSelectedShape();
  if (!shape) return;
  sliderInteractionActive = true;
  sliderInteractionShape = shape;
  sliderInteractionBefore = captureShapeState(shape);
}

void PropertiesPanel::endSliderInteraction() {
  if (!sliderInteractionActive) return;
  auto shape = sliderInteractionShape;
  auto before = sliderInteractionBefore;
  sliderInteractionActive = false;
  sliderInteractionShape = nullptr;
  if (!shape || shape != canvas->getSelectedShape()) return;
  auto after = captureShapeState(shape);
  pushShapeStateCommand(shape, before, after);
}

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
  updating = false;
}

void PropertiesPanel::applyToShape() {
  if (updating) return;
  updatePreviews();
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
