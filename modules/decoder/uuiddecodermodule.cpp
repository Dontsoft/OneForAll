#include "uuiddecodermodule.hpp"

#include "uuiddecodermodulewidget.hpp"

UUIDDecoderModule::UUIDDecoderModule(QObject *parent)
    : Module(tr("UUID Decoder"), "com.dontsoft.devtools.uuiddecoder",
             QObject ::tr("Decoders"), QIcon(), new UUIDDecoderModuleWidget(),
             parent),
      Loggable("com.dontsoft.devtools.uuiddecoder")
{

}
