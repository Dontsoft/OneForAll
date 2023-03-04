#ifndef MODULENAVIGATIONFILTERMODEL_HPP
#define MODULENAVIGATIONFILTERMODEL_HPP

#include <QSortFilterProxyModel>

class ModuleNavigationFilterModel : public QSortFilterProxyModel
{
public:
    explicit ModuleNavigationFilterModel(QObject *parent = nullptr);
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

private:
    bool filterByRegularExpression(const QRegularExpression& expression,
                                   const QModelIndex& index) const;
    bool filterByString(const QString& text, const QModelIndex& index) const;
};

#endif // MODULENAVIGATIONFILTERMODEL_HPP
