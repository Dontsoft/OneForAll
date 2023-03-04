#include "devtoolsmainwindow.h"
#include "./ui_devtoolsmainwindow.h"

#include <QSortFilterProxyModel>

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
      _navigationModel(new ModuleNavigationFilterModel(this))
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
    connect(ui->navigationListView->selectionModel(),
            &QItemSelectionModel::currentChanged, this,
            &DevToolsMainWindow::handleTreeViewSelection);
    connect(ui->navigationFilter, &QLineEdit::textChanged, this,
            &DevToolsMainWindow::searchStringChanged);
    ui->navigationListView->setCurrentIndex(
        ui->navigationListView->model()->index(0, 0));
    handleTreeViewSelection(ui->navigationListView->model()->index(0, 0),
                            QModelIndex());
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
