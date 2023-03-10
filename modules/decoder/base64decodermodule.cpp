#include "base64decodermodule.hpp"

#include "base64decodermodulewidget.hpp"

Base64DecoderModule::Base64DecoderModule(QObject *parent)
    : Module{tr("Base64 Decoder"),
             "com.dontsoft.devtools.base64decoder",
             QObject::tr("Decoders"),
             QIcon(),
             new Base64DecoderModuleWidget(),
             parent},
      Loggable("com.dontsoft.devtools.base64decoder")
{
    const auto widgetCasted
        = static_cast<Base64DecoderModuleWidget *>(getWidget());
    connect(widgetCasted, &Base64DecoderModuleWidget::decode, this,
            &Base64DecoderModule::decode);
    connect(this, &Base64DecoderModule::decoded, widgetCasted,
            &Base64DecoderModuleWidget::decoded);
}

void Base64DecoderModule::decode(const QString &encodedString)
{
    if (auto result = QByteArray::fromBase64Encoding(encodedString.toUtf8()))
    {
        emit decoded(*result);
    }
}
