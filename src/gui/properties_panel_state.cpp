// properties_panel_state.cpp — snapshot/apply helpers for property commands
#include "gui/properties_panel.h"

#include <memory>

#include "gui/canvas.h"
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
