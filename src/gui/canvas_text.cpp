// canvas_text.cpp
// text editing logic for canvas - double click to edit, inline editor, and
// command creation for undo/redo

#include <QLineEdit>
#include <algorithm>

#include "gui/canvas.h"
#include "shapes/text_shape.h"
#include "tools/command.h"
#include "tools/shape_property_command.h"

// start text editing by showing an inline editor and caching original text
// if editing a new text shape created by double click, use a draft shape to
// track whether the shape was committed or discarded
void Canvas::finalizeTextEditing() {
  if (!textEditing) return;
  auto txt = std::dynamic_pointer_cast<TextShape>(selectedShape);
  if (txt && textEditor) txt->setText(textEditor->text().toStdString());
  endTextEditing();
  if (!txt) {
    textDraftShape = nullptr;
    return;
  }
  bool isDraft = (textDraftShape && textDraftShape == selectedShape);
  if (isDraft) {
    // empty draft text is discarded
    if (txt->getText().empty()) {
      shapes.erase(std::remove(shapes.begin(), shapes.end(), selectedShape),
                   shapes.end());
      setSelectedShape(nullptr);
    } else {
      // committed new text is recorded as add shape action
      pushCommand(std::make_unique<AddShapeCommand>(selectedShape));
    }
    textDraftShape = nullptr;
  } else {
    // existing text edit is captured as before after property snapshot
    ShapePropertyState before;
    before.fillColor = txt->getFillColor();
    before.strokeColor = txt->getStrokeColor();
    before.strokeWidth = txt->getStrokeWidth();
    before.hasTextStyle = true;
    before.fontFamily = txt->getFontFamily();
    before.fontSize = txt->getFontSize();
    before.hasTextContent = true;
    before.textContent = textBeforeEditing;

    auto after = before;
    after.textContent = txt->getText();
    if (after != before) {
      pushCommand(
          std::make_unique<ShapePropertyCommand>(selectedShape, before, after));
    }
  }
  update();
}
