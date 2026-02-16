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
    // If the "Apply" checkboxes are enabled, applying the preview values
    // is a user action and must be undoable. Snapshot previous values and
    // push a LambdaCommand when anything changes.
    bool applied = false;
    std::string oldFill = shape->getFillColor();
    std::string oldStroke = shape->getStrokeColor();
    double oldWidth = shape->getStrokeWidth();

    std::string newFill = oldFill;
    std::string newStroke = oldStroke;
    double newWidth = oldWidth;

    if (fillApplyCheck->isChecked()) {
      newFill = getEffectiveFill().name(QColor::HexArgb).toStdString();
      applied = (newFill != oldFill);
    }
    if (strokeApplyCheck->isChecked()) {
      newStroke = getEffectiveStroke().name(QColor::HexArgb).toStdString();
      newWidth = widthSlider->value();
      applied = applied || (newStroke != oldStroke) || (newWidth != oldWidth);
    }

    if (applied) {
      // apply immediately
      shape->setFillColor(newFill);
      shape->setStrokeColor(newStroke);
      shape->setStrokeWidth(newWidth);
      // make change undoable
      canvas->pushCommand(std::make_unique<LambdaCommand>(
          [shape, oldFill, oldStroke, oldWidth](Canvas* c) {
            shape->setFillColor(oldFill);
            shape->setStrokeColor(oldStroke);
            shape->setStrokeWidth(oldWidth);
            c->update();
          },
          [shape, newFill, newStroke, newWidth](Canvas* c) {
            shape->setFillColor(newFill);
            shape->setStrokeColor(newStroke);
            shape->setStrokeWidth(newWidth);
            c->update();
          }));
      canvas->update();
    }

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

  // Snapshot previous values
  std::string oldFill = shape->getFillColor();
  std::string oldStroke = shape->getStrokeColor();
  double oldWidth = shape->getStrokeWidth();
  std::string newFill = getEffectiveFill().name(QColor::HexArgb).toStdString();
  std::string newStroke =
      getEffectiveStroke().name(QColor::HexArgb).toStdString();
  double newWidth = widthSlider->value();

  // Apply new values
  shape->setFillColor(newFill);
  shape->setStrokeColor(newStroke);
  shape->setStrokeWidth(newWidth);

  // Other properties
  if (auto rr = std::dynamic_pointer_cast<RoundedRectangle>(shape))
    rr->setCornerRadius(cornerSlider->value());
  if (auto hex = std::dynamic_pointer_cast<Hexagon>(shape))
    hex->setPointyTop(pointyTopBtn->isChecked());
  if (auto text = std::dynamic_pointer_cast<TextShape>(shape)) {
    std::string oldFamily = text->getFontFamily();
    int oldSize = text->getFontSize();
    std::string newFamily = fontCombo->currentFont().family().toStdString();
    int newSize = fontSizeSpin->value();
    text->setFontFamily(newFamily);
    text->setFontSize(newSize);

    // push command for font changes
    canvas->pushCommand(std::make_unique<LambdaCommand>(
        [shape, oldFamily, oldSize](Canvas* c) {
          if (auto t = std::dynamic_pointer_cast<TextShape>(shape)) {
            t->setFontFamily(oldFamily);
            t->setFontSize(oldSize);
            c->update();
          }
        },
        [shape, newFamily, newSize](Canvas* c) {
          if (auto t = std::dynamic_pointer_cast<TextShape>(shape)) {
            t->setFontFamily(newFamily);
            t->setFontSize(newSize);
            c->update();
          }
        }));
  }

  // push a command for color/width changes
  canvas->pushCommand(std::make_unique<LambdaCommand>(
      [shape, oldFill, oldStroke, oldWidth](Canvas* c) {
        shape->setFillColor(oldFill);
        shape->setStrokeColor(oldStroke);
        shape->setStrokeWidth(oldWidth);
        c->update();
      },
      [shape, newFill, newStroke, newWidth](Canvas* c) {
        shape->setFillColor(newFill);
        shape->setStrokeColor(newStroke);
        shape->setStrokeWidth(newWidth);
        c->update();
      }));

  updatePreviews();
  canvas->update();
}
