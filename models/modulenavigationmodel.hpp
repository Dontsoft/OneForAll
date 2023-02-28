#ifndef MODULENAVIGATIONMODEL_HPP
#define MODULENAVIGATIONMODEL_HPP

#include <QAbstractListModel>
#include <QMap>
#include <QSharedPointer>

class ModuleRegistry;

class ModuleNavigationModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum UserRole
    {
        ModuleIdentifierRole = Qt::UserRole,
        ModuleCategoryRole
    };

    explicit ModuleNavigationModel(
        const QSharedPointer<ModuleRegistry> &registry,
        QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QSharedPointer<ModuleRegistry> _registry;
};

#endif // MODULENAVIGATIONMODEL_HPP
