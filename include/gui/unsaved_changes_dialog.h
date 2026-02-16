// unsaved_changes_dialog.h
#pragma once

#include <QString>

class QWidget;

// result of what the prompt shows
enum class UnsavedChoice { Save, Discard, Cancel };

// show dialogue asking user what they want to do
UnsavedChoice promptUnsavedChanges(QWidget* parent, const QString& question);
