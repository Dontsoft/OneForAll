#ifndef CONFIG_YAML_FILE_ENGINE_HPP
#define CONFIG_YAML_FILE_ENGINE_HPP

#include "configfileengine.hpp"

class ConfigYamlFileEngine : public ConfigFileEngine {
   public:
    explicit ConfigYamlFileEngine(const QString& filename,
                                  const ConfigEngine::FlushPolicy& flushPolicy);

   protected:
    void flushImplementation(const QHash<QString, QVariant>& values) override;
    QHash<QString, QVariant> getAll() const override;
};

#endif
