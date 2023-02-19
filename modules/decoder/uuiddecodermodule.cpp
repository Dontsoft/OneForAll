#include "uuiddecodermodule.hpp"

#include "uuiddecodermodulewidget.hpp"

UUIDDecoderModule::UUIDDecoderModule(QObject *parent)
    : Module(tr("UUID Decoder"), "com.dontsoft.devtools.uuiddecoder",
             QObject ::tr("Decoders"), QIcon(), parent),
      Loggable("com.dontsoft.devtools.uuiddecoder"),
      _widget(new UUIDDecoderModuleWidget())
{

}

QWidget *UUIDDecoderModule::getWidget() const
{
    return _widget;
}
