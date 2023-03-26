#ifndef ABOUTDIALOG_HPP
#define ABOUTDIALOG_HPP

#include <QDialog>
#include <QItemSelection>
#include <QSharedPointer>

#include "core/dependant.hpp"

class AppInformation;

namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog,
                    public Dependant<QSharedPointer<AppInformation>>
{
    Q_OBJECT

public:
    explicit AboutDialog(const AboutDialog::Dependency &dependency,
                         QWidget *parent = nullptr);
    ~AboutDialog();

private slots:
    void thirdPartySelectionChanged(const QItemSelection &current,
                                    const QItemSelection &deselected);

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_HPP
