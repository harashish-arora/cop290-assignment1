// properties_panel.h
// fill/stroke properties editor with colour picker
#pragma once
#include <QColor>
#include <QWidget>
#include <memory>

#include "tools/shape_property_command.h"

// forward declarations
class QSlider;
class QSpinBox;
class QPushButton;
class QLabel;
class QCheckBox;
class QFontComboBox;
class QHBoxLayout;
class Canvas;
class GraphicsObject;

// exposes controls for fill/stroke and common shape properties.
class PropertiesPanel : public QWidget {
  Q_OBJECT
 public:
  explicit PropertiesPanel(Canvas* canvas, QWidget* parent = nullptr);

 public slots:
  // refresh controls from currently selected shape
  void refreshFromSelection();

 private slots:
  void applyToShape();            // apply current control values to the shape
  void onFillPresetClicked();     // user clicked a fill color preset
  void onStrokePresetClicked();   // user clicked a stroke color preset
  void onFillPreviewClicked();    // user clicked the fill preview button
  void onStrokePreviewClicked();  // user clicked the stroke preview button

 private:
  Canvas* canvas;         // non-owning pointer to the central Canvas
  bool updating = false;  // true when UI is updating programmatically

  QColor fillRgb_{255, 255, 255};
  QColor strokeRgb_{0, 0, 0};

  // fill controls
  QPushButton* fillPreview;
  QSlider* fillAlphaSlider;
  QSpinBox* fillAlphaSpin;
  QCheckBox* fillApplyCheck;

  // stroke control
  QPushButton* strokePreview;
  QSlider* strokeAlphaSlider;
  QSpinBox* strokeAlphaSpin;
  QSlider* widthSlider;
  QSpinBox* widthSpin;
  QCheckBox* strokeApplyCheck;

  // other shape controls for some shapes
  QSlider* cornerSlider;
  QSpinBox* cornerSpin;
  QPushButton* flatTopBtn;
  QPushButton* pointyTopBtn;
  QFontComboBox* fontCombo;
  QSpinBox* fontSizeSpin;

  // helpers for effective colours with transparency
  QColor getEffectiveFill() const;
  QColor getEffectiveStroke() const;

  // capture and apply shape properties for undo/redo
  ShapePropertyState captureShapeState(
      const std::shared_ptr<GraphicsObject>& shape) const;
  void applyStateToShape(const std::shared_ptr<GraphicsObject>& shape,
                         const ShapePropertyState& state) const;
  void pushShapeStateCommand(const std::shared_ptr<GraphicsObject>& shape,
                             const ShapePropertyState& before,
                             const ShapePropertyState& after);

  // batch updates for slider interactions
  void beginSliderInteraction();
  void endSliderInteraction();
  void syncCreationDefaults();
  void updatePreviews();
  void setupGrid(QHBoxLayout* mainRow);
  void connectSignals();

  // state while user is interactins with sliders
  bool sliderInteractionActive = false;
  std::shared_ptr<GraphicsObject> sliderInteractionShape = nullptr;
  ShapePropertyState sliderInteractionBefore;
};
