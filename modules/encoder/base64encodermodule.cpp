#include "base64encodermodule.hpp"

#include "base64encodermodulewidget.hpp"

Base64EncoderModule::Base64EncoderModule(QObject *parent)
    : Module{tr("Base64 Encoder"),
             "com.dontsoft.devtools.base64encoder",
             QObject::tr("Encoders"),
             QIcon(),
             new Base64EncoderModuleWidget(),
             parent},
      Loggable("com.dontsoft.devtools.base64encoder")
{
    const auto widgetCasted
        = static_cast<Base64EncoderModuleWidget *>(getWidget());

    connect(widgetCasted, &Base64EncoderModuleWidget::base64OptionsChanged,
            this, &Base64EncoderModule::base64OptionsChanged);
    connect(widgetCasted, &Base64EncoderModuleWidget::encode, this,
            &Base64EncoderModule::encode);

    connect(this, &Base64EncoderModule::encoded, widgetCasted,
            &Base64EncoderModuleWidget::encoded);
}

void Base64EncoderModule::encode(const QByteArray &byteArray)
{
    emit encoded(byteArray.toBase64(_options));
}

void Base64EncoderModule::base64OptionsChanged(
    QByteArray::Base64Options options)
{
    _options = options;
}
