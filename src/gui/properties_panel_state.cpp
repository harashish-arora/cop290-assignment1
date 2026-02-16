// properties_panel_state.cpp
// capture and apply shape properties for undo/redo and slider interactions

#include <memory>

#include "gui/canvas.h"
#include "gui/properties_panel.h"
#include "shapes/hexagon.h"
#include "shapes/rounded_rectangle.h"
#include "shapes/text_shape.h"

// get current shape properties into a snapshot object
ShapePropertyState PropertiesPanel::captureShapeState(
    const std::shared_ptr<GraphicsObject>& shape) const {
  ShapePropertyState state;
  if (!shape) return state;
  state.fillColor = shape->getFillColor();
  state.strokeColor = shape->getStrokeColor();
  state.strokeWidth = shape->getStrokeWidth();
  // checks for special properties of shapes
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

// apply a snapshot to a shape including shape specific fields
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

// helper to push one property command to undo stack when changed
void PropertiesPanel::pushShapeStateCommand(
    const std::shared_ptr<GraphicsObject>& shape,
    const ShapePropertyState& before, const ShapePropertyState& after) {
  if (!shape || before == after) return;
  canvas->pushCommand(
      std::make_unique<ShapePropertyCommand>(shape, before, after));
}

// start slider interaction session to combine many value changed events
void PropertiesPanel::beginSliderInteraction() {
  if (updating || sliderInteractionActive) return;
  auto shape = canvas->getSelectedShape();
  if (!shape) return;
  sliderInteractionActive = true;
  sliderInteractionShape = shape;
  sliderInteractionBefore = captureShapeState(shape);
}

// end slider session and push one combined command
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
