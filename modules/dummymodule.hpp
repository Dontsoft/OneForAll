#ifndef DUMMYMODULE_HPP
#define DUMMYMODULE_HPP

#include "module.hpp"
#include <QObject>

class DummyModule : public Module
{
    Q_OBJECT
public:
    explicit DummyModule(int index, const QString& category,
                         QObject* parent = nullptr);
    QWidget* getWidget() const override;

private:
    QWidget* _widget;
};

#endif // DUMMYMODULE_HPP
