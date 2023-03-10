#ifndef BASE64DECODERMODULE_HPP
#define BASE64DECODERMODULE_HPP

#include "log/loggable.hpp"
#include "modules/module.hpp"

class Base64DecoderModule : public Module, public Loggable
{
    Q_OBJECT
public:
    explicit Base64DecoderModule(QObject* parent = nullptr);

signals:
    void decoded(const QByteArray& byteArray);

private slots:
    void decode(const QString& encodedString);
};

#endif // BASE64DECODERMODULE_HPP
