#include "modulenavigationmodel.hpp"

#include <QDebug>
#include <QFont>
#include <QSize>

#include "modules/moduleregistry.hpp"

constexpr quintptr combine(int category, int module)
{
    return ((unsigned)category << 8) | (unsigned)module;
}
constexpr int get_category(quintptr internalId)
{
    return internalId >> 8;
}
constexpr int get_module(quintptr internalId)
{
    return internalId & 0xFF;
}

QString decodeInternalId(quintptr internalId)
{
    return QString("Category: %1, Module: %2")
        .arg(get_category(internalId) - 1)
        .arg(get_module(internalId) - 1);
}

namespace internal
{
    ModelRegistryAdapter::ModelRegistryAdapter(
        const QSharedPointer<ModuleRegistry> &registry, QObject *parent)
        : QObject(parent), _registry(registry)
    {
    }

    bool ModelRegistryAdapter::isCategory(quintptr internalId) const
    {
        return get_module(internalId) == 0;
    }

    bool ModelRegistryAdapter::isCategoryAndHasChildren(
        quintptr internalId) const
    {

        return isCategory(internalId)
               && _registry
                          ->getModulesForCategory(
                              getCategoryRowForInternalId(internalId))
                          .size()
                      > 0;
    }

    quintptr ModelRegistryAdapter::getInternalIdForCategory(int row) const
    {
        return combine(row + 1, 0);
    }

    quintptr ModelRegistryAdapter::getInternalIdForModule(int categoryRow,
                                                          int row) const
    {
        return combine(categoryRow + 1, row + 1);
    }

    int ModelRegistryAdapter::getCategoryRowForInternalId(
        quintptr internalId) const
    {
        return get_category(internalId) - 1;
    }

    int ModelRegistryAdapter::getModuleRowForInternalId(
        quintptr internalId) const
    {
        return get_module(internalId) - 1;
    }

    QString ModelRegistryAdapter::getCategoryForInternalId(
        quintptr internalId) const
    {
        const auto categoryList = _registry->getCategoryRows();
        const auto categoryRow = getCategoryRowForInternalId(internalId);
        return categoryList[categoryRow];
    }

    QString ModelRegistryAdapter::getModuleForInternalId(
        quintptr internalId) const
    {
        const auto categoryRow = getCategoryRowForInternalId(internalId);
        const auto moduleRow = getModuleRowForInternalId(internalId);
        const auto moduleList = _registry->getModulesForCategory(categoryRow);
        return moduleList[moduleRow];
    }

    QString ModelRegistryAdapter::getModuleNameForInternalId(
        quintptr internalId) const
    {
        const auto identifier = getModuleForInternalId(internalId);
        return _registry->getModuleNameForIdentifier(identifier);
    }

    QIcon ModelRegistryAdapter::getModuleIconForInternalId(
        quintptr internalId) const
    {
        const auto identifier = getModuleForInternalId(internalId);
        return _registry->getModuleIconForIdentifier(identifier);
    }

} // namespace internal

ModuleNavigationModel::ModuleNavigationModel(
    const QSharedPointer<ModuleRegistry> &registry, QObject *parent)
    : QAbstractItemModel(parent), _registry(registry),
      _adapter(new internal::ModelRegistryAdapter(_registry, this))
{
}

QModelIndex ModuleNavigationModel::index(int row, int column,
                                         const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }
    if (!parent.isValid())
    {
        // Index is Category
        const auto internalId = _adapter->getInternalIdForCategory(row);
        return createIndex(row, column, internalId);
    }
    return createIndex(row, column,
                       _adapter->getInternalIdForModule(parent.row(), row));
}

QModelIndex ModuleNavigationModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }
    const auto isCategory = _adapter->isCategory(index.internalId());
    if (isCategory)
    {
        return QModelIndex();
    }
    const auto categoryRow
        = _adapter->getCategoryRowForInternalId(index.internalId());
    return createIndex(categoryRow, 0,
                       _adapter->getInternalIdForCategory(categoryRow));
}

bool ModuleNavigationModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return true;
    }
    return _adapter->isCategoryAndHasChildren(parent.internalId());
}

int ModuleNavigationModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return _registry->getCategoryRows().size();
    }
    if (_adapter->isCategory(parent.internalId()))
    {
        const int parentRow = parent.row();
        return _registry->getModulesForCategory(parentRow).size();
    }
    return 0;
}

int ModuleNavigationModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ModuleNavigationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    switch (role)
    {
    case Qt::DisplayRole: {

        if (_adapter->isCategory(index.internalId()))
        {
            return _adapter->getCategoryForInternalId(index.internalId());
        }
        return _adapter->getModuleNameForInternalId(index.internalId());
    }
    case Qt::DecorationRole: {
        if (!_adapter->isCategory(index.internalId()))
        {
            return _adapter->getModuleIconForInternalId(index.internalId());
        }
    }
    case Qt::FontRole: {
        auto font = QFont();
        if (_adapter->isCategory(index.internalId()))
        {
            font.setPointSize(16);
            font.setBold(true);
        }
        return font;
    }
    case Qt::SizeHintRole: {
        if (_adapter->isCategory(index.internalId()))
        {
            return QSize(0, 48);
        }
        return QSize(0, 32);
    }
    case ModuleNavigationModel::IsCategoryRole: {
        return _adapter->isCategory(index.internalId());
    }
    case ModuleNavigationModel::CategoryHasModulesRole: {
        return _adapter->isCategoryAndHasChildren(index.internalId());
    }
    case ModuleNavigationModel::ModuleIdentifierRole: {
        if (!_adapter->isCategory(index.internalId()))
        {
            return _adapter->getModuleForInternalId(index.internalId());
        }
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
    if (_adapter->isCategory(index.internalId()))
    {
        return QAbstractItemModel::flags(index) & ~Qt::ItemIsSelectable;
    }
    return QAbstractItemModel::flags(index);
}
