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
    QSharedPointer<Module> getModule(
        const Module::Identifier& identifier) const;
    QSharedPointer<Module> getModule(int row) const;
    QList<QSharedPointer<Module>> getModules() const;
    int getModuleCount() const;

public slots:
    void addAsFavorite(const Module::Identifier& identifier);
    void removeAsFavorite(const Module::Identifier& identifier);
    void changeFavoritePosition(const Module::Identifier& identifier,
                                int newPosition);

signals:

private:
    QList<Module::Identifier> _favorites;
    QMap<Module::Identifier, QSharedPointer<Module>> _moduleLookupTable;
};
#endif // MODULEREGISTRY_HPP
