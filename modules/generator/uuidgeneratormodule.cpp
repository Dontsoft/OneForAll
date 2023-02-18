#include "uuidgeneratormodule.hpp"

#include "uuidgeneratormodulewidget.hpp"

UUIDGeneratorModule::UUIDGeneratorModule(QObject *parent)
    : Module(tr("UUID Generator"), "com.dontsoft.devtools.uuidgenerator",
             tr("Generators"), QIcon(), parent),
      _widget(new UUIDGeneratorModuleWidget())
{

}

QWidget *UUIDGeneratorModule::getWidget() const
{
    return _widget;
}
