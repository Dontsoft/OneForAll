#ifndef PREFERENCESDIALOG_HPP
#define PREFERENCESDIALOG_HPP

#include <QDialog>
#include <QSharedPointer>

#include "core/dependant.hpp"
#include "log/loggable.hpp"

class ConfigEngine;
class PathRegistry;
class ModuleRegistry;

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog,
                          public Dependant<QSharedPointer<ConfigEngine>,
                                           QSharedPointer<PathRegistry>,
                                           QSharedPointer<ModuleRegistry>>,
                          public Loggable
{
    Q_OBJECT

public:
    static const int RELOAD_REQUIRED = 0x1337;

    explicit PreferencesDialog(const PreferencesDialog::Dependency &dependency,
                               QWidget *parent = nullptr);
    ~PreferencesDialog();

    void applyClicked();

private:
    Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_HPP
