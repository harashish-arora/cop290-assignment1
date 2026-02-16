// unsaved_changes_dialog.h — shared save/discard/cancel prompt
#pragma once

#include <QString>

class QWidget;

enum class UnsavedChoice { Save, Discard, Cancel };

UnsavedChoice promptUnsavedChanges(QWidget* parent, const QString& question);
