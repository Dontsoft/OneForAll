#include "aboutdialog.hpp"
#include "QtWidgets/qapplication.h"
#include "ui_aboutdialog.h"

#include <iostream>

#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QItemSelectionModel>

#include "utility/appinformation.hpp"

enum ThirdPartyUserRoles
{
    Name = Qt::UserRole,
    Author,
    SupportMail,
    SupportWebpage,
    SupportGithub,
    Type,
    Version,
    Version_Long,
    LogoPath,
    License,
    ThirdPartyType
};

AboutDialog::AboutDialog(const Dependency& dependency, QWidget* parent)
    : QDialog(parent), Dependant<QSharedPointer<AppInformation>>{dependency},
      ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    connect(ui->thirdPartyListWidget->selectionModel(),
            &QItemSelectionModel::selectionChanged, this,
            &AboutDialog::thirdPartySelectionChanged);

    auto appInformation = getDependency<0>();
    for (auto it = appInformation->thirdPartyBegin();
         it != appInformation->thirdPartyEnd(); ++it)
    {
        auto item = new QListWidgetItem(it->values[AppInformation::Type::Name]);

        item->setData(ThirdPartyUserRoles::Name,
                      it->values[AppInformation::Type::Name]);
        item->setData(ThirdPartyUserRoles::Author,
                      it->values[AppInformation::Type::Author]);
        item->setData(ThirdPartyUserRoles::SupportMail,
                      it->values[AppInformation::Type::SupportMail]);
        item->setData(ThirdPartyUserRoles::SupportWebpage,
                      it->values[AppInformation::Type::SupportWebpage]);
        item->setData(ThirdPartyUserRoles::SupportGithub,
                      it->values[AppInformation::Type::SupportGithub]);
        item->setData(ThirdPartyUserRoles::Type,
                      it->values[AppInformation::Type::Type]);
        item->setData(ThirdPartyUserRoles::Version,
                      it->values[AppInformation::Type::Version]);
        item->setData(ThirdPartyUserRoles::Version_Long,
                      it->values[AppInformation::Type::Version_Long]);
        item->setData(ThirdPartyUserRoles::LogoPath,
                      it->values[AppInformation::Type::LogoPath]);
        item->setData(ThirdPartyUserRoles::License,
                      it->values[AppInformation::Type::License]);
        switch (it->type)
        {
        case AppInformation::ThirdPartyInformation::Type::Library:
            item->setData(ThirdPartyUserRoles::ThirdPartyType, tr("Library"));
            break;
        }

        ui->thirdPartyListWidget->addItem(item);
    }
    if (ui->thirdPartyListWidget->count() > 0)
    {
        ui->thirdPartyListWidget->setCurrentRow(0);
    }

    const QString headerText
        = tr("<h3>%1 (%2)</h3>")
              .arg(appInformation->get(AppInformation::Type::Name),
                   appInformation->get(AppInformation::Type::Version));
    ui->aboutInformationLabel->setText(
        tr("%1"
           ""
           "Based on Qt %2 (%3)<br>"
           "<a href='about:qt'>About Qt</a>"
           "<br><br>"
           "<b>Author</b>: %4<br>"
           "<b>E-Mail</b>: <a href='mailto:%5'>%5</a><br>"
           "<b>GitHub</b>: <a href='%6'>%6</a><br>"
           "<b>Webpage</b>: <a href='%7'>%7</a>")
            .arg(headerText, QT_VERSION_STR, AppInformation::compilerString(),
                 appInformation->get(AppInformation::Type::Author),
                 appInformation->get(AppInformation::Type::SupportMail),
                 appInformation->get(AppInformation::Type::SupportGithub),
                 appInformation->get(AppInformation::Type::SupportWebpage)));

    connect(ui->aboutInformationLabel, &QLabel::linkActivated, this,
            [=](const QString& link) {
                if (link == "about:qt")
                {
                    QApplication::aboutQt();
                }
                else
                {
                    QDesktopServices::openUrl(link);
                }
            });

    const auto license = appInformation->get(AppInformation::Type::License);
    if (license.startsWith(":"))
    {
        QFile f(license);
        if (f.open(QIODevice::Text | QIODevice::ReadOnly))
        {
            ui->aboutLicenseEdit->setPlainText(f.readAll());
        }
    }
    else
    {
        ui->aboutLicenseEdit->setPlainText(license);
    }
    const auto logoPath = appInformation->get(AppInformation::Type::LogoPath);
    if (!logoPath.isEmpty())
    {
        ui->logoLabel->setPixmap(QPixmap(logoPath));
    }
    setWindowTitle(tr("About %1").arg(QApplication::applicationName()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::thirdPartySelectionChanged(const QItemSelection& current,
                                             const QItemSelection& deselected)
{
    Q_UNUSED(deselected)
    const auto index = current.indexes().first();
    if (!index.isValid())
    {
        return;
    }
    const auto name = index.data(ThirdPartyUserRoles::Name).toString();
    const auto author = index.data(ThirdPartyUserRoles::Author).toString();
    const auto supportMail
        = index.data(ThirdPartyUserRoles::SupportMail).toString();
    const auto supportWebpage
        = index.data(ThirdPartyUserRoles::SupportWebpage).toString();
    const auto supportGithub
        = index.data(ThirdPartyUserRoles::SupportGithub).toString();
    const auto version = index.data(ThirdPartyUserRoles::Version).toString();
    const auto versionLong
        = index.data(ThirdPartyUserRoles::Version_Long).toString();
    const auto license = index.data(ThirdPartyUserRoles::License).toString();
    const auto thirdPartyType
        = index.data(ThirdPartyUserRoles::ThirdPartyType).toString();

    const QString headerText
        = tr("<h3>%1: %2 (%3)</h3>")
              .arg(thirdPartyType, name,
                   versionLong.isEmpty() ? version : versionLong);

    QStringList content{};
    if (!author.isEmpty())
    {
        content.append(tr("<b>Author</b>: %1").arg(author));
    }
    if (!supportWebpage.isEmpty())
    {
        content.append(tr("<b>Webpage</b>: %1").arg(supportWebpage));
    }
    if (!supportGithub.isEmpty())
    {
        content.append(tr("<b>GitHub</b>: %1").arg(supportGithub));
    }
    if (!supportMail.isEmpty())
    {
        content.append(tr("<b>E-Mail</b>: mailto:%1").arg(supportMail));
    }

    const QString thirdPartyText = tr("%1"
                                      "%2")
                                       .arg(headerText, content.join("<br>"));
    ui->thirdPartyInformationLabel->setText(thirdPartyText);
    if (license.startsWith(":"))
    {
        QFile f(license);
        if (f.open(QIODevice::Text | QIODevice::ReadOnly))
        {
            ui->thirdPartyLicenseEdit->setPlainText(f.readAll());
        }
        else
        {
            ui->thirdPartyLicenseEdit->clear();
        }
    }
    else
    {
        ui->thirdPartyLicenseEdit->setPlainText(license);
    }
}
