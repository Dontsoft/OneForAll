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
    if (!_modules.contains(module->getCategory()))
    {
        _modules.insert(module->getCategory(), {});
    }
    _modules[module->getCategory()].append(module->getIdentifier());
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

QList<QString> ModuleRegistry::getCategoryRows() const
{
    QList<QString> rows{tr("Favorites")};
    for (auto it = std::begin(_modules); it != std::end(_modules); ++it)
    {
        rows.append(it.key());
    }
    return rows;
}

QList<Module::Identifier> ModuleRegistry::getModulesForCategory(
    int categoryIndex) const
{
    if (categoryIndex == 0)
    {
        return _favorites;
    }
    if ((categoryIndex - 1) >= _modules.size())
    {
        return {};
    }
    const auto category = _modules.keys()[categoryIndex - 1];
    return _modules[category];
}

QString ModuleRegistry::getModuleNameForIdentifier(
    const Module::Identifier& identifier) const
{
    const auto _module = _moduleLookupTable.value(identifier);
    if (_module.isNull())
    {
        return QString();
    }
    return _module->getName();
}

QIcon ModuleRegistry::getModuleIconForIdentifier(
    const Module::Identifier& identifier) const
{
    const auto _module = _moduleLookupTable.value(identifier);
    if (_module.isNull())
    {
        return QIcon();
    }
    return _module->getIcon();
}

QSharedPointer<Module> ModuleRegistry::getModuleForIdentifier(
    const Module::Identifier& identifier) const
{
    return _moduleLookupTable.value(identifier);
}

QList<QSharedPointer<Module>> ModuleRegistry::getModules() const
{
    return _moduleLookupTable.values();
}
