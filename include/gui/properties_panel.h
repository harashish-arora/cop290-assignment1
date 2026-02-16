// properties_panel.h — Dual fill/stroke properties editor with color picker
#pragma once
#include <QColor>
#include <QWidget>
#include <memory>

#include "tools/shape_property_command.h"

class QSlider;
class QSpinBox;
class QPushButton;
class QLabel;
class QCheckBox;
class QFontComboBox;
class QHBoxLayout;
class Canvas;
class GraphicsObject;

class PropertiesPanel : public QWidget {
  Q_OBJECT
 public:
  explicit PropertiesPanel(Canvas* canvas, QWidget* parent = nullptr);

 public slots:
  void refreshFromSelection();

 private slots:
  void applyToShape();
  void onFillPresetClicked();
  void onStrokePresetClicked();
  void onFillPreviewClicked();
  void onStrokePreviewClicked();

 private:
  Canvas* canvas;
  bool updating = false;

  // Tracked base RGB (alpha lives in the sliders)
  QColor fillRgb_{255, 255, 255};
  QColor strokeRgb_{0, 0, 0};

  // Fill section
  QPushButton* fillPreview;
  QSlider* fillAlphaSlider;
  QSpinBox* fillAlphaSpin;
  QCheckBox* fillApplyCheck;

  // Stroke section
  QPushButton* strokePreview;
  QSlider* strokeAlphaSlider;
  QSpinBox* strokeAlphaSpin;
  QSlider* widthSlider;
  QSpinBox* widthSpin;
  QCheckBox* strokeApplyCheck;

  // Shape-specific controls (always visible in grid)
  QSlider* cornerSlider;
  QSpinBox* cornerSpin;
  QPushButton* flatTopBtn;
  QPushButton* pointyTopBtn;
  QFontComboBox* fontCombo;
  QSpinBox* fontSizeSpin;

  QColor getEffectiveFill() const;
  QColor getEffectiveStroke() const;
  ShapePropertyState captureShapeState(
      const std::shared_ptr<GraphicsObject>& shape) const;
  void applyStateToShape(const std::shared_ptr<GraphicsObject>& shape,
                         const ShapePropertyState& state) const;
  void pushShapeStateCommand(const std::shared_ptr<GraphicsObject>& shape,
                             const ShapePropertyState& before,
                             const ShapePropertyState& after);
  void beginSliderInteraction();
  void endSliderInteraction();
  void syncCreationDefaults();
  void updatePreviews();
  void setupGrid(QHBoxLayout* mainRow);
  void connectSignals();

  bool sliderInteractionActive = false;
  std::shared_ptr<GraphicsObject> sliderInteractionShape = nullptr;
  ShapePropertyState sliderInteractionBefore;
};
