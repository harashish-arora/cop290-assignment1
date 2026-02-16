// properties_panel_connect.cpp — Signal-slot wiring
#include <QFontComboBox>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

#include "gui/properties_panel.h"

void PropertiesPanel::connectSignals() {
  // Fill Opacity
  connect(fillAlphaSlider, &QSlider::valueChanged, fillAlphaSpin,
          &QSpinBox::setValue);
  connect(fillAlphaSpin, QOverload<int>::of(&QSpinBox::valueChanged),
          fillAlphaSlider, &QSlider::setValue);
  connect(fillAlphaSlider, &QSlider::valueChanged, this,
          &PropertiesPanel::applyToShape);

  // Stroke Opacity
  connect(strokeAlphaSlider, &QSlider::valueChanged, strokeAlphaSpin,
          &QSpinBox::setValue);
  connect(strokeAlphaSpin, QOverload<int>::of(&QSpinBox::valueChanged),
          strokeAlphaSlider, &QSlider::setValue);
  connect(strokeAlphaSlider, &QSlider::valueChanged, this,
          &PropertiesPanel::applyToShape);

  // Width
  connect(widthSlider, &QSlider::valueChanged, widthSpin, &QSpinBox::setValue);
  connect(widthSpin, QOverload<int>::of(&QSpinBox::valueChanged), widthSlider,
          &QSlider::setValue);
  connect(widthSlider, &QSlider::valueChanged, this,
          &PropertiesPanel::applyToShape);

  // Corner Radius
  connect(cornerSlider, &QSlider::valueChanged, cornerSpin,
          &QSpinBox::setValue);
  connect(cornerSpin, QOverload<int>::of(&QSpinBox::valueChanged), cornerSlider,
          &QSlider::setValue);
  connect(cornerSlider, &QSlider::valueChanged, this,
          &PropertiesPanel::applyToShape);

  // Orientation
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

  // Font
  connect(fontCombo, &QFontComboBox::currentFontChanged, this,
          [this](const QFont&) { applyToShape(); });
  connect(fontSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int) { applyToShape(); });
}
