#ifndef UUIDGENERATORMODULE_HPP
#define UUIDGENERATORMODULE_HPP

#include "modules/module.hpp"
#include <QObject>

class UUIDGeneratorModule : public Module
{
    Q_OBJECT
public:
    explicit UUIDGeneratorModule(QObject *parent = nullptr);
    QWidget* getWidget() const override;

private:
    QWidget* _widget;
};

#endif // UUIDGENERATORMODULE_HPP
