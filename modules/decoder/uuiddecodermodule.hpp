#ifndef UUIDDECODERMODULE_HPP
#define UUIDDECODERMODULE_HPP
#include "log/loggable.hpp"
#include "modules/module.hpp"

#include <QObject>

class UUIDDecoderModule : public Module, public Loggable
{
    Q_OBJECT
public:
    explicit UUIDDecoderModule(QObject *parent = nullptr);
};

#endif // UUIDDECODERMODULE_HPP
