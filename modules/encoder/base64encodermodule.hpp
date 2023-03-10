#ifndef BASE64ENCODERMODULE_HPP
#define BASE64ENCODERMODULE_HPP

#include "log/loggable.hpp"
#include "modules/module.hpp"
#include <QObject>

class Base64EncoderModule : public Module, public Loggable
{
    Q_OBJECT
public:
    explicit Base64EncoderModule(QObject* parent = nullptr);
public slots:
    void encode(const QByteArray& byteArray);
    void base64OptionsChanged(QByteArray::Base64Options options);

signals:
    void encoded(const QString& encodedContent);

private:
    QByteArray::Base64Options _options = QByteArray::Base64Encoding;
};

#endif // BASE64ENCODERMODULE_HPP
