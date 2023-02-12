#ifndef CONFIG_NATIVE_FILE_ENGINE_HPP
#define CONFIG_NATIVE_FILE_ENGINE_HPP

#include "configfileengine.hpp"

class ConfigNativeFileEngine : public ConfigFileEngine {
   public:
    explicit ConfigNativeFileEngine(
        const QString& filename, const ConfigEngine::FlushPolicy& flushPolicy);

   protected:
    void flushImplementation(const QHash<QString, QVariant>& values) override;
    QHash<QString, QVariant> getAll() const override;
};

#endif
