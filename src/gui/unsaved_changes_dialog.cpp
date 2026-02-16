// unsaved changes dialog cpp
// result of what the prompt shows

#include "gui/unsaved_changes_dialog.h"

#include <QAbstractButton>
#include <QMessageBox>
#include <QPushButton>

// show unsaved changes prompt and map clicked button to enum choice
UnsavedChoice promptUnsavedChanges(QWidget* parent, const QString& question) {
  QMessageBox box(parent);
  box.setIcon(QMessageBox::NoIcon);
  box.setWindowTitle("Unsaved Changes");
  box.setText(question);
  auto* saveBtn = box.addButton("Save", QMessageBox::AcceptRole);
  box.addButton("Don't Save", QMessageBox::DestructiveRole);
  auto* cancelBtn = box.addButton("Cancel", QMessageBox::RejectRole);
  box.setDefaultButton(cancelBtn);
  box.exec();
  auto* clicked = box.clickedButton();
  if (!clicked || clicked == cancelBtn) return UnsavedChoice::Cancel;
  if (clicked == saveBtn) return UnsavedChoice::Save;
  return UnsavedChoice::Discard;
}
