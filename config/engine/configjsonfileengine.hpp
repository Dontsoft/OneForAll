#ifndef CONFIG_JSON_FILE_ENGINE_HPP
#define CONFIG_JSON_FILE_ENGINE_HPP

#include "configfileengine.hpp"

class ConfigJsonFileEngine : public ConfigFileEngine {
   public:
    explicit ConfigJsonFileEngine(const QString& filename,
                                  const ConfigEngine::FlushPolicy& flushPolicy);

   protected:
    void flushImplementation(const QHash<QString, QVariant>& values) override;
    QHash<QString, QVariant> getAll() const override;
};

#endif
