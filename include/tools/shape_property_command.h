// shape_property_command.h
#pragma once

#include <memory>
#include <string>

#include "tools/command.h"

struct ShapePropertyState {
  std::string fillColor;
  std::string strokeColor;
  double strokeWidth = 1.0;
  bool hasCornerRadius = false;
  double cornerRadius = 0.0;
  bool hasPointyTop = false;
  bool pointyTop = false;
  bool hasTextStyle = false;
  std::string fontFamily;
  int fontSize = 16;
  bool hasTextContent = false;
  std::string textContent;

  bool operator==(const ShapePropertyState& other) const;
  bool operator!=(const ShapePropertyState& other) const;
};

class ShapePropertyCommand : public Command {
 private:
  std::shared_ptr<GraphicsObject> shape;
  ShapePropertyState before;
  ShapePropertyState after;

  void applyState(const ShapePropertyState& state, Canvas* canvas);

 public:
  ShapePropertyCommand(std::shared_ptr<GraphicsObject> shape,
                       ShapePropertyState before, ShapePropertyState after);
  void undo(Canvas* canvas) override;
  void redo(Canvas* canvas) override;
};
