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

void PropertiesPanel::refreshFromSelection() {
  updating = true;
  auto shape = canvas->getSelectedShape();

  if (shape) {
    bool applied = false;
    if (fillApplyCheck->isChecked()) {
      shape->setFillColor(
          getEffectiveFill().name(QColor::HexArgb).toStdString());
      applied = true;
    }
    if (strokeApplyCheck->isChecked()) {
      shape->setStrokeColor(
          getEffectiveStroke().name(QColor::HexArgb).toStdString());
      shape->setStrokeWidth(widthSlider->value());
      applied = true;
    }
    if (applied) canvas->update();

    QColor fc(QString::fromStdString(shape->getFillColor()));
    fillRgb_ = QColor(fc.red(), fc.green(), fc.blue());
    fillAlphaSlider->setValue(fc.alpha());

    QColor sc(QString::fromStdString(shape->getStrokeColor()));
    strokeRgb_ = QColor(sc.red(), sc.green(), sc.blue());
    strokeAlphaSlider->setValue(sc.alpha());
    widthSlider->setValue(static_cast<int>(shape->getStrokeWidth()));

    if (auto rr = std::dynamic_pointer_cast<RoundedRectangle>(shape))
      cornerSlider->setValue(static_cast<int>(rr->getCornerRadius()));
    if (auto hex = std::dynamic_pointer_cast<Hexagon>(shape)) {
      flatTopBtn->setChecked(!hex->isPointyTop());
      pointyTopBtn->setChecked(hex->isPointyTop());
    }
    if (auto text = std::dynamic_pointer_cast<TextShape>(shape)) {
      fontCombo->setCurrentFont(
          QFont(QString::fromStdString(text->getFontFamily())));
      fontSizeSpin->setValue(text->getFontSize());
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

  shape->setFillColor(getEffectiveFill().name(QColor::HexArgb).toStdString());
  shape->setStrokeColor(
      getEffectiveStroke().name(QColor::HexArgb).toStdString());
  shape->setStrokeWidth(widthSlider->value());

  if (auto rr = std::dynamic_pointer_cast<RoundedRectangle>(shape))
    rr->setCornerRadius(cornerSlider->value());
  if (auto hex = std::dynamic_pointer_cast<Hexagon>(shape))
    hex->setPointyTop(pointyTopBtn->isChecked());
  if (auto text = std::dynamic_pointer_cast<TextShape>(shape)) {
    text->setFontFamily(fontCombo->currentFont().family().toStdString());
    text->setFontSize(fontSizeSpin->value());
  }

  updatePreviews();
  canvas->update();
}
