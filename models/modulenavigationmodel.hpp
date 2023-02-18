#ifndef MODULENAVIGATIONMODEL_HPP
#define MODULENAVIGATIONMODEL_HPP

#include <QAbstractItemModel>
#include <QMap>
#include <QSharedPointer>

class ModuleRegistry;
namespace internal
{
    class ModelRegistryAdapter : public QObject
    {
    public:
        explicit ModelRegistryAdapter(
            const QSharedPointer<ModuleRegistry> &registry,
            QObject *parent = nullptr);

        bool isCategory(quintptr internalId) const;
        bool isCategoryAndHasChildren(quintptr internalId) const;
        quintptr getInternalIdForCategory(int row) const;
        quintptr getInternalIdForModule(int categoryRow, int row) const;

        int getCategoryRowForInternalId(quintptr internalId) const;
        int getModuleRowForInternalId(quintptr internalId) const;

        QString getCategoryForInternalId(quintptr internalId) const;
        QString getModuleForInternalId(quintptr internalId) const;
        QString getModuleNameForInternalId(quintptr internalId) const;
        QIcon getModuleIconForInternalId(quintptr internalId) const;

    private:
        QSharedPointer<ModuleRegistry> _registry;
    };
}; // namespace internal

class ModuleNavigationModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum UserRole
    {
        IsCategoryRole = Qt::UserRole,
        CategoryHasModulesRole,
        ModuleIdentifierRole
    };

    explicit ModuleNavigationModel(
        const QSharedPointer<ModuleRegistry> &registry,
        QObject *parent = nullptr);

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QSharedPointer<ModuleRegistry> _registry;
    internal::ModelRegistryAdapter *_adapter;
};

#endif // MODULENAVIGATIONMODEL_HPP
