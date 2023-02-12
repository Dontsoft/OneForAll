#include "configfileengine.hpp"

ConfigFileEngine::ConfigFileEngine(const QString& filename,
                                   const ConfigEngine::FlushPolicy& flushPolicy)
    : ConfigEngine(flushPolicy), _filename(filename) {}

QString ConfigFileEngine::getFilename() const { return _filename; }
