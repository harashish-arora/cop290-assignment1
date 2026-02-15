// properties_panel.h — Color/stroke properties editor (horizontal layout)
#pragma once
#include <QWidget>
#include <memory>
class QSlider;
class QSpinBox;
class QPushButton;
class QLabel;
class QFontComboBox;
class Canvas;
class GraphicsObject;
class QGridLayout;

class PropertiesPanel : public QWidget {
  Q_OBJECT
 public:
  explicit PropertiesPanel(Canvas* canvas, QWidget* parent = nullptr);

 public slots:
  void refreshFromSelection();

 private slots:
  void applyToShape();
  void onPresetClicked();

 private:
  Canvas* canvas;
  bool updating = false;
  QPushButton* fillBtn;
  QPushButton* strokeBtn;
  QSlider *rSlider, *gSlider, *bSlider, *alphaSlider, *widthSlider;
  QSpinBox *rSpin, *gSpin, *bSpin, *alphaSpin, *widthSpin;
  QWidget* preview;
  QLabel* widthLabel;

  // Shape-specific section (right side)
  QWidget* shapeSection;
  // Rounded rectangle controls
  QLabel* cornerLabel;
  QSlider* cornerSlider;
  QSpinBox* cornerSpin;
  // Hexagon controls
  QPushButton* flatTopBtn;
  QPushButton* pointyTopBtn;
  // Text controls
  QLabel* fontLabel;
  QFontComboBox* fontCombo;
  QLabel* fontSizeLabel;
  QSpinBox* fontSizeSpin;

  QGridLayout* sliderGrid;

  bool isStrokeMode() const;
  QColor currentColor() const;
  void updateDisplay();
};
