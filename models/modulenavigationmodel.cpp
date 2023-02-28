#include "modulenavigationmodel.hpp"

#include <QDebug>
#include <QFont>
#include <QSize>

#include "modules/moduleregistry.hpp"

ModuleNavigationModel::ModuleNavigationModel(
    const QSharedPointer<ModuleRegistry> &registry, QObject *parent)
    : QAbstractListModel(parent), _registry(registry)
{
}

int ModuleNavigationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _registry->getModuleCount();
}

QVariant ModuleNavigationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    const auto row = index.row();
    const auto module = _registry->getModule(row);
    switch (role)
    {
    case Qt::DisplayRole: {
        return module->getName();
    }
    case Qt::DecorationRole: {
        QPixmap icon = module->getIcon().pixmap(16, 16);
        if (icon.isNull())
        {
            icon = QPixmap(16, 16);
            icon.fill(Qt::transparent);
        }
        return icon;
    }
    case Qt::SizeHintRole: {
        return QSize(128, 32);
    }
    case ModuleNavigationModel::ModuleCategoryRole: {
        return module->getCategory();
    }
    case ModuleNavigationModel::ModuleIdentifierRole: {
        return module->getIdentifier();
    }
    }
    return QVariant();
}

Qt::ItemFlags ModuleNavigationModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::ItemFlags();
    }
    return QAbstractItemModel::flags(index);
}
