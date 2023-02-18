#include "dummymodule.hpp"

#include <QLabel>

DummyModule::DummyModule(int index, const QString &category, QObject *parent)
    : Module(QString("Dummy %1").arg(index),
             QString("com.dontsoft.devtools.dummy_%1").arg(index), category,
             QIcon(), parent),
      _widget(new QLabel(QString("Dummy %1").arg(index)))
{

}

QWidget *DummyModule::getWidget() const
{
    return _widget;
}
