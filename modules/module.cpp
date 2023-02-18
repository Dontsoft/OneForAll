#include "module.hpp"

Module::Module(const QString &name, const QString &identifier,
               const QString &category, const QIcon &_icon, QObject *parent)
    : QObject{parent}, _name(name), _identifier(identifier),
      _category(category), _icon(QIcon())
{

}
