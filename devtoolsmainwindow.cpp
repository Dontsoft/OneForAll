#include "devtoolsmainwindow.h"
#include "./ui_devtoolsmainwindow.h"

#include <QSortFilterProxyModel>

#include "delegates/styledlistdelegate.hpp"
#include "dialogs/aboutdialog.hpp"
#include "dialogs/preferencesdialog.hpp"
#include "global.hpp"
#include "models/modulenavigationfiltermodel.hpp"
#include "models/modulenavigationmodel.hpp"

#include "spdlog/spdlog.h"

enum Dependencies
{
    ConfigEngineDependency = 0,
    PathRegistryDependency = 1,
    AppInformationDependency = 2,
    ModuleRegistryDependency = 3
};

DevToolsMainWindow::DevToolsMainWindow(const Dependency& dependency,
                                       QWidget* parent)
    : QMainWindow(parent),
      Dependant<QSharedPointer<ConfigEngine>, QSharedPointer<PathRegistry>,
                QSharedPointer<AppInformation>, QSharedPointer<ModuleRegistry>>(
          dependency),
      Loggable("Mainwindow"), ui(new Ui::DevToolsMainWindow),
      _navigationModel(new ModuleNavigationFilterModel(
          {getDependency<Dependencies::ConfigEngineDependency>()}, this))
{
    ui->setupUi(this);
    const auto moduleRegistry
        = getDependency<Dependencies::ModuleRegistryDependency>();
    const auto moduleList = moduleRegistry->getModules();
    for (const auto& module : moduleList)
    {
        ui->contentWidget->addWidget(module->getWidget());
    }
    _navigationModel->setSourceModel(
        new ModuleNavigationModel(moduleRegistry, this));
    ui->navigationListView->setModel(_navigationModel);
    ui->navigationListView->setItemDelegate(new StyledListDelegate());

    connect(ui->navigationListView->selectionModel(),
            &QItemSelectionModel::currentChanged, this,
            &DevToolsMainWindow::handleTreeViewSelection);
    connect(ui->navigationFilter, &QLineEdit::textChanged, this,
            &DevToolsMainWindow::searchStringChanged);
    ui->navigationListView->setCurrentIndex(
        ui->navigationListView->model()->index(0, 0));
    handleTreeViewSelection(ui->navigationListView->model()->index(0, 0),
                            QModelIndex());

#ifdef Q_OS_WIN
    QMenu* menu = new QMenu("File");
#else

    QMenu* applicationMenu = new QMenu("ApplicationMenu");

    QAction* aboutAction = new QAction("About");
    connect(aboutAction, &QAction::triggered, this,
            &DevToolsMainWindow::showAbout);

    QAction* preferencesAction = new QAction("config");
    connect(preferencesAction, &QAction::triggered, this,
            &DevToolsMainWindow::showPreferences);

    applicationMenu->addAction(aboutAction);
    applicationMenu->addAction(preferencesAction);

    ui->menubar->addMenu(applicationMenu);

#endif

    QMenu* fileMenu = new QMenu(tr("File"));
#ifdef Q_OS_WIN
    QAction* preferencesAction = new QAction("Preferences...");
    connect(preferencesAction, &QAction::triggered, this,
            &DevToolsMainWindow::showPreferences);
#endif
    fileMenu->addSeparator();
    fileMenu->addAction(preferencesAction);

    ui->menubar->addMenu(fileMenu);

    QMenu* helpMenu = new QMenu(tr("Help"));
#ifdef Q_OS_WIN
    QAction* aboutAction = new QAction(tr("About"));
    connect(aboutAction, &QAction::triggered, this,
            &DevToolsMainWindow::showAbout);
#endif
    QAction* helpAction = new QAction(tr("Help"));
    QAction* contactAction = new QAction(tr("Contact"));

    helpMenu->addAction(helpAction);
    helpMenu->addAction(contactAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);

    ui->menubar->addMenu(helpMenu);

    // ui->menubar->addAction(new QAction("config"));
}

DevToolsMainWindow::~DevToolsMainWindow() { delete ui; }

void DevToolsMainWindow::updateWindowTitle(const QString& title)
{
    setWindowTitle(title);
}

void DevToolsMainWindow::handleTreeViewSelection(const QModelIndex& current,
                                                 const QModelIndex& previous)
{
    Q_UNUSED(previous)
    const auto moduleIdentifier = qvariant_cast<QString>(_navigationModel->data(
        current, ModuleNavigationModel::ModuleIdentifierRole));
    if (!moduleIdentifier.isEmpty())
    {
        const auto moduleRegistry
            = getDependency<Dependencies::ModuleRegistryDependency>();
        QSharedPointer<Module> module
            = moduleRegistry->getModule(moduleIdentifier);
        if (!module.isNull())
        {
            ui->contentWidget->setCurrentWidget(module->getWidget());
            const auto appInformation
                = getDependency<Dependencies::AppInformationDependency>();
            setWindowTitle(QString("%1 - %2").arg(
                appInformation->get(AppInformation::Type::Name),
                module->getName()));
        }
    }
}

void DevToolsMainWindow::searchStringChanged(const QString& text)
{
    _navigationModel->setFilterRegularExpression(text);
}

void DevToolsMainWindow::showAbout()
{
    const auto appInformation
        = getDependency<Dependencies::AppInformationDependency>();
    AboutDialog* dialog = new AboutDialog({appInformation}, this);
    connect(dialog, &QDialog::finished, this,
            [=](int) { dialog->deleteLater(); });
    dialog->setModal(true);
    dialog->show();
    dialog->raise();
}

void DevToolsMainWindow::showPreferences()
{
    const auto configEngine
        = getDependency<Dependencies::ConfigEngineDependency>();
    const auto pathRegistry
        = getDependency<Dependencies::PathRegistryDependency>();
    const auto moduleRegistry
        = getDependency<Dependencies::ModuleRegistryDependency>();
    PreferencesDialog* dialog = new PreferencesDialog(
        {configEngine, pathRegistry, moduleRegistry}, this);
    connect(dialog, &QDialog::finished, this, [=](int result) {
        dialog->deleteLater();
        if (result == PreferencesDialog::RELOAD_REQUIRED)
        {
            QApplication::exit(RESTART_RETURN_CODE);
        }
    });
    dialog->setModal(true);
    dialog->show();
    dialog->raise();
}
