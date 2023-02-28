#include "moduleregistry.hpp"

#include <QDebug>
#include <QtGlobal>

#include "spdlog/spdlog.h"

ModuleRegistry::ModuleRegistry(QObject* parent)
    : QObject{parent}, Loggable("Module Registry")
{

}

bool ModuleRegistry::addModule(const QSharedPointer<Module>& module)
{
    if (_moduleLookupTable.contains(module->getIdentifier()))
    {
        logger->error("Identifier {} already registered",
                      module->getIdentifier());
        return false;
    }
    _moduleLookupTable.insert(module->getIdentifier(), module);
    return true;
}

void ModuleRegistry::addAsFavorite(const Module::Identifier& identifier)
{
    _favorites.append(identifier);
}

void ModuleRegistry::removeAsFavorite(const Module::Identifier& identifier)
{
    _favorites.removeAll(identifier);
}

void ModuleRegistry::changeFavoritePosition(
    const Module::Identifier& identifier, int newPosition)
{
    if (!_favorites.contains(identifier))
    {
        return;
    }
    const auto currentIdentifierAtPosition = _favorites.at(newPosition);
    const int currentPosition = _favorites.indexOf(identifier);
    _favorites[currentPosition] = currentIdentifierAtPosition;
    _favorites[newPosition] = identifier;
}

QSharedPointer<Module> ModuleRegistry::getModule(
    const Module::Identifier& identifier) const
{
    return _moduleLookupTable.value(identifier);
}

QSharedPointer<Module> ModuleRegistry::getModule(int row) const
{
    return getModules()[row];
}

QList<QSharedPointer<Module>> ModuleRegistry::getModules() const
{
    return _moduleLookupTable.values();
}

int ModuleRegistry::getModuleCount() const
{
    return _moduleLookupTable.size();
}
