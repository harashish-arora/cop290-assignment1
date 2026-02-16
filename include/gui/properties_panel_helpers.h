// properties_panel_helpers.h
#pragma once
#include <QColor>
#include <QString>

// forward declarations
class QSlider;
class QSpinBox;
class QFrame;
class QVBoxLayout;
class QPushButton;
class QCheckBox;
class PropertiesPanel;

// helper functions used by the properties panel
QSlider* makeSlider(int lo, int hi, int val);  // slider with range/value
QSpinBox* makeSpin(int lo, int hi, int val);   // spinbox with range/value
QFrame* vSep();                                // vertical separator frame
QString previewBtnStyle(const QColor& c);      // color preview button style
QString swatchStyle(const QColor& c);          // stylesheet for a color swatch

extern const char* kCheckStyle;
extern const char* kBoldLabel;
extern const char* kSectionTitle;

// Build a vertical column with color presets and a preview button.
QVBoxLayout* buildColorColumn(const char* title, QPushButton*& previewOut,
                              QCheckBox*& checkOut, const QColor presets[],
                              PropertiesPanel* panel, const char* presetSlot);
