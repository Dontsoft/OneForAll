#include "devtoolsmainwindow.h"
#include "./ui_devtoolsmainwindow.h"

#include <QSortFilterProxyModel>

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
      _navigationModel(new QSortFilterProxyModel(this))
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
    ui->navigationTreeView->setModel(_navigationModel);
    connect(ui->navigationTreeView->selectionModel(),
            &QItemSelectionModel::currentChanged, this,
            &DevToolsMainWindow::handleTreeViewSelection);
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
            = moduleRegistry->getModuleForIdentifier(moduleIdentifier);
        if (!module.isNull())
        {
            ui->contentWidget->setCurrentWidget(module->getWidget());
        }
    }
}

void DevToolsMainWindow::on_navigationTreeView_clicked(const QModelIndex& index)
{
    const auto isCategoryAndHasChildren
        = qvariant_cast<bool>(_navigationModel->data(
            index, ModuleNavigationModel::CategoryHasModulesRole));
    if (!isCategoryAndHasChildren)
    {
        return;
    }
    ui->navigationTreeView->setExpanded(
        index, !ui->navigationTreeView->isExpanded(index));
}
