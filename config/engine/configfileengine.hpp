#ifndef CONFIG_FILE_ENGINE_HPP
#define CONFIG_FILE_ENGINE_HPP

#include "config/configengine.hpp"

class ConfigFileEngine : public ConfigEngine {
   public:
    QString getFilename() const;

   protected:
    explicit ConfigFileEngine(const QString &filename,
                              const ConfigEngine::FlushPolicy &flushPolicy);

   private:
    const QString _filename;
};

#endif
