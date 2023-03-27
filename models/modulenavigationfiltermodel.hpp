#ifndef MODULENAVIGATIONFILTERMODEL_HPP
#define MODULENAVIGATIONFILTERMODEL_HPP

#include <QSortFilterProxyModel>

#include "config/configlistener.hpp"
#include "core/dependant.hpp"

class ConfigEngine;

class ModuleNavigationFilterModel
    : public QSortFilterProxyModel,
      public Dependant<QSharedPointer<ConfigEngine>>,
      public ConfigListener
{
public:
    explicit ModuleNavigationFilterModel(const Dependency& dependency,
                                         QObject* parent = nullptr);
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex& sourceParent) const override;

    void notificationReceived(ConfigEngine* sender, const QString& key,
                              const QPair<QVariant, QVariant>& params) override;

private:
    bool filterByRegularExpression(const QRegularExpression& expression,
                                   const QModelIndex& index) const;
    bool filterByString(const QString& text, const QModelIndex& index) const;
};

#endif // MODULENAVIGATIONFILTERMODEL_HPP
