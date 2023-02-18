#ifndef MODULEREGISTRY_HPP
#define MODULEREGISTRY_HPP

#include <QMap>
#include <QObject>
#include <QSharedPointer>

#include "log/loggable.hpp"
#include "modules/module.hpp"

class ModuleRegistry : public QObject, public Loggable
{
    Q_OBJECT
public:
    explicit ModuleRegistry(QObject* parent = nullptr);

    bool addModule(const QSharedPointer<Module>& module);
    QList<QString> getCategoryRows() const;
    QList<Module::Identifier> getModulesForCategory(int categoryIndex) const;
    QString getModuleNameForIdentifier(
        const Module::Identifier& identifier) const;
    QIcon getModuleIconForIdentifier(
        const Module::Identifier& identifier) const;
    QSharedPointer<Module> getModuleForIdentifier(
        const Module::Identifier& identifier) const;
    QList<QSharedPointer<Module>> getModules() const;

public slots:
    void addAsFavorite(const Module::Identifier& identifier);
    void removeAsFavorite(const Module::Identifier& identifier);
    void changeFavoritePosition(const Module::Identifier& identifier,
                                int newPosition);

signals:

private:
    QList<Module::Identifier> _favorites;
    QMap<QString, QList<Module::Identifier>> _modules;
    QMap<Module::Identifier, QSharedPointer<Module>> _moduleLookupTable;
};
#endif // MODULEREGISTRY_HPP
