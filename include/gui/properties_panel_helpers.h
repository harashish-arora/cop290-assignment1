// properties_panel_helpers.h — Shared UI factory functions
#pragma once
#include <QColor>
#include <QString>

class QSlider;
class QSpinBox;
class QFrame;
class QVBoxLayout;
class QPushButton;
class QCheckBox;
class PropertiesPanel;

QSlider* makeSlider(int lo, int hi, int val);
QSpinBox* makeSpin(int lo, int hi, int val);
QFrame* vSep();
QString previewBtnStyle(const QColor& c);
QString swatchStyle(const QColor& c);

extern const char* kCheckStyle;
extern const char* kBoldLabel;
extern const char* kSectionTitle;

QVBoxLayout* buildColorColumn(const char* title, QPushButton*& previewOut,
                              QCheckBox*& checkOut, const QColor presets[],
                              PropertiesPanel* panel, const char* presetSlot);
