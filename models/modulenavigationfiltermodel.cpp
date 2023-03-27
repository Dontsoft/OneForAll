#include "modulenavigationfiltermodel.hpp"
#include "modulenavigationmodel.hpp"

#include "config/configengine.hpp"

ModuleNavigationFilterModel::ModuleNavigationFilterModel(
    const Dependency& dependency, QObject* parent)
    : QSortFilterProxyModel{parent},
      Dependant<QSharedPointer<ConfigEngine>>{dependency}, ConfigListener{}
{
    auto configEngine = getDependency<0>();
    configEngine->subscribe(this);
}

void ModuleNavigationFilterModel::notificationReceived(
    ConfigEngine* sender, const QString& key,
    const QPair<QVariant, QVariant>& params)
{
    Q_UNUSED(sender)
    Q_UNUSED(key)
    Q_UNUSED(params)
    if (key.startsWith("LoadedModules/"))
    {
        invalidateFilter();
    }
}

bool ModuleNavigationFilterModel::filterAcceptsRow(
    int sourceRow, const QModelIndex& sourceParent) const
{
    auto regularExpression = filterRegularExpression();
    const auto index = sourceModel()->index(sourceRow, 0, sourceParent);
    const QString identifier
        = index.data(ModuleNavigationModel::ModuleIdentifierRole).toString();
    auto configEngine = getDependency<0>();
    if (configEngine->get<bool>("LoadedModules/" + identifier, true) == false)
    {
        return false;
    }
    if (regularExpression.isValid())
    {
        return filterByRegularExpression(regularExpression, index);
    }
    return filterByString(regularExpression.pattern(), index);
}

bool ModuleNavigationFilterModel::filterByRegularExpression(
    const QRegularExpression& expression, const QModelIndex& index) const
{
    const QString category
        = index.data(ModuleNavigationModel::ModuleCategoryRole).toString();
    const QString identifier
        = index.data(ModuleNavigationModel::ModuleIdentifierRole).toString();
    const QString name = index.data(Qt::DisplayRole).toString();
    return category.contains(expression) || identifier.contains(expression)
           || name.contains(expression);
}

bool ModuleNavigationFilterModel::filterByString(const QString& text,
                                                 const QModelIndex& index) const
{
    if (text.isEmpty())
    {
        return true;
    }
    const QString category
        = index.data(ModuleNavigationModel::ModuleCategoryRole).toString();
    const QString identifier
        = index.data(ModuleNavigationModel::ModuleIdentifierRole).toString();
    const QString name = index.data(Qt::DisplayRole).toString();
    return category.contains(text, Qt::CaseInsensitive)
           || identifier.contains(text, Qt::CaseInsensitive)
           || name.contains(text, Qt::CaseInsensitive);
}
