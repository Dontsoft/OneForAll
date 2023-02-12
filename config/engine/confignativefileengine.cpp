#include "confignativefileengine.hpp"

#include <QSettings>

ConfigNativeFileEngine::ConfigNativeFileEngine(
    const QString& filename, const ConfigEngine::FlushPolicy& flushPolicy)
    : ConfigFileEngine(filename, flushPolicy) {}

void ConfigNativeFileEngine::flushImplementation(
    const QHash<QString, QVariant>& values) {
#ifdef Q_OS_WIN
    QSettings settings(getFilename(), QSettings::IniFormat);
#else
    QSettings settings(getFilename(), QSettings::NativeFormat);
#endif
    for (auto it = values.constBegin(); it != values.constEnd(); ++it)
        settings.setValue(it.key(), it.value());
}

QHash<QString, QVariant> ConfigNativeFileEngine::getAll() const {
    QHash<QString, QVariant> values;
#ifdef Q_OS_WIN
    QSettings settings(getFilename(), QSettings::IniFormat);
#else
    QSettings settings(getFilename(), QSettings::NativeFormat);
#endif
    const QStringList keys = settings.allKeys();
    for (const auto& key : keys)
        values.insert(key, settings.value(key));
    return values;
}
