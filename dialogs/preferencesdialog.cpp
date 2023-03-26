#include "preferencesdialog.hpp"
#include "ui_preferencesdialog.h"

#include <QPushButton>

PreferencesDialog::PreferencesDialog(const Dependency &dependency,
                                     QWidget *parent)
    : QDialog(parent), Dependant{dependency}, Loggable("Preferences Dialog"),
      ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Preferences - %1").arg(QApplication::applicationName()));

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setDefault(true);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this,
            [=](QAbstractButton *button) {
                if (button == ui->buttonBox->button(QDialogButtonBox::Cancel))
                {
                    reject();
                }
                else if (button
                         == ui->buttonBox->button(QDialogButtonBox::Apply))
                {
                    applyClicked();
                }
            });
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::applyClicked()
{
    done(PreferencesDialog::RELOAD_REQUIRED);
}
