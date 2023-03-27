#ifndef CONFIG_LISTENER_HPP
#define CONFIG_LISTENER_HPP

#include "configengine.hpp"

#include "core/observer.hpp"

class ConfigListener
    : public Observer<ConfigEngine, QPair<QVariant, QVariant>> {};

#endif
