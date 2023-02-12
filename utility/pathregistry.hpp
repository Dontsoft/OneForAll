#ifndef PATH_REGISTRY_HPP
#define PATH_REGISTRY_HPP

#include <QDir>
#include <QMap>
#include <QString>

class PathRegistry {
   public:
    enum PathType { Data = 0x00, Config = 0x01, Log = 0x02, UserType = 0xFFFF };

    bool set(int type, QString path);

    template <int type>
    QString get() const;

    template <int type, typename... Args>
    QString get(Args... args) const;

    template <int type>
    std::string native() const;

    template <int type, typename... Args>
    std::string native(Args... args) const;

    bool make(const QString& path) const;
    bool cache();

   protected:
    virtual QString data() const = 0;
    virtual QString config() const = 0;
    virtual QString log() const = 0;

   private:
    QMap<int, QString> _registry;
};

template <int type>
QString PathRegistry::get() const {
    if (!_registry.contains(type)) {
        if constexpr (type == PathRegistry::Data)
            return data();
        else if constexpr (type == PathRegistry::Config)
            return config();
        else if constexpr (type == PathRegistry::Log)
            return log();
        else
            return QString();
    }
    return _registry[type];
}

template <int type, typename... Args>
QString PathRegistry::get(Args... args) const {
    if (!_registry.contains(type)) {
        if constexpr (type == PathRegistry::Data)
            return data();
        else if constexpr (type == PathRegistry::Config)
            return config();
        else if constexpr (type == PathRegistry::Log)
            return log();
        else
            return QString();
    }
    auto pathWithArgs = _registry[type].arg(args...);
    if (!make(pathWithArgs))
        return QString();
    return pathWithArgs;
}

template <int type>
std::string PathRegistry::native() const {
    return QDir::toNativeSeparators(get<type>()).toStdString();
}

template <int type, typename... Args>
std::string PathRegistry::native(Args... args) const {
    return QDir::toNativeSeparators(get<type>(args...)).toStdString();
}

class UserOnlyPathRegistry : public PathRegistry {
   protected:
    QString data() const override;
    QString config() const override;
    QString log() const override;

   private:
    mutable QString _cachedData;
    mutable QString _cachedConfig;
    mutable QString _cachedLog;
};

class SystemPathRegistry : public PathRegistry {
   protected:
    QString data() const override;
    QString config() const override;
    QString log() const override;

   private:
    mutable QString _cachedData;
    mutable QString _cachedConfig;
    mutable QString _cachedLog;
};

#endif
