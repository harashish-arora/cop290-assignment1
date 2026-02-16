// properties panel dialogs cpp
// implementation for properties panel dialogs

#include <QColorDialog>
#include <QPushButton>
#include <QSlider>

#include "gui/properties_panel.h"

// preset click updates fill rgb then applies to selected shape
void PropertiesPanel::onFillPresetClicked() {
  auto* btn = qobject_cast<QPushButton*>(sender());
  if (!btn) return;
  fillRgb_ = btn->property("presetColor").value<QColor>();
  updatePreviews();
  applyToShape();
}

// preset click updates stroke rgb then applies to selected shape
void PropertiesPanel::onStrokePresetClicked() {
  auto* btn = qobject_cast<QPushButton*>(sender());
  if (!btn) return;
  strokeRgb_ = btn->property("presetColor").value<QColor>();
  updatePreviews();
  applyToShape();
}

// open fill color dialog then sync rgb and alpha controls
void PropertiesPanel::onFillPreviewClicked() {
  QColor initial = getEffectiveFill();
  QColor chosen = QColorDialog::getColor(initial, this, "Fill Color",
                                         QColorDialog::ShowAlphaChannel);
  if (!chosen.isValid()) return;
  fillRgb_ = QColor(chosen.red(), chosen.green(), chosen.blue());
  updating = true;
  fillAlphaSlider->setValue(chosen.alpha());
  updating = false;
  updatePreviews();
  applyToShape();
}

// open stroke color dialog then sync rgb and alpha controls
void PropertiesPanel::onStrokePreviewClicked() {
  QColor initial = getEffectiveStroke();
  QColor chosen = QColorDialog::getColor(initial, this, "Stroke Color",
                                         QColorDialog::ShowAlphaChannel);
  if (!chosen.isValid()) return;
  strokeRgb_ = QColor(chosen.red(), chosen.green(), chosen.blue());
  updating = true;
  strokeAlphaSlider->setValue(chosen.alpha());
  updating = false;
  updatePreviews();
  applyToShape();
}
