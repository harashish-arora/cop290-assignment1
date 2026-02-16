// shape_property_command.cpp
// implementation of undo command for shape property changes

#include "tools/shape_property_command.h"

#include "gui/canvas.h"
#include "shapes/hexagon.h"
#include "shapes/rounded_rectangle.h"
#include "shapes/text_shape.h"

// compare full property snapshots to detect changes
bool ShapePropertyState::operator==(const ShapePropertyState& other) const {
  return fillColor == other.fillColor && strokeColor == other.strokeColor &&
         strokeWidth == other.strokeWidth &&
         hasCornerRadius == other.hasCornerRadius &&
         cornerRadius == other.cornerRadius &&
         hasPointyTop == other.hasPointyTop && pointyTop == other.pointyTop &&
         hasTextStyle == other.hasTextStyle && fontFamily == other.fontFamily &&
         fontSize == other.fontSize && hasTextContent == other.hasTextContent &&
         textContent == other.textContent;
}

bool ShapePropertyState::operator!=(const ShapePropertyState& other) const {
  return !(*this == other);
}

// store shape pointer and before after snapshots
ShapePropertyCommand::ShapePropertyCommand(
    std::shared_ptr<GraphicsObject> shape, ShapePropertyState before,
    ShapePropertyState after)
    : shape(std::move(shape)),
      before(std::move(before)),
      after(std::move(after)) {}

// apply one snapshot to shape
// guarded by has fields for shape specific properties
void ShapePropertyCommand::applyState(const ShapePropertyState& state,
                                      Canvas* canvas) {
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
  if (state.hasTextContent) {
    if (auto text = std::dynamic_pointer_cast<TextShape>(shape))
      text->setText(state.textContent);
  }
  canvas->update();
}

// undo replays before snapshot
void ShapePropertyCommand::undo(Canvas* canvas) { applyState(before, canvas); }

// redo replays after snapshot
void ShapePropertyCommand::redo(Canvas* canvas) { applyState(after, canvas); }
