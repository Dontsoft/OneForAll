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
    QWidget* getWidget() const override;

signals:

private:
    QWidget* _widget;
};

#endif // UUIDDECODERMODULE_HPP
