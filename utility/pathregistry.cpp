#include "pathregistry.hpp"

#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QStandardPaths>
#include <QtGlobal>

bool PathRegistry::set(int type, QString path) {
    if (_registry.contains(type))
        return false;
    if (path.contains("%DATA%", Qt::CaseInsensitive))
        path.replace("%DATA%", data());
    else if (path.contains("%CONFIG%", Qt::CaseInsensitive))
        path.replace("%CONFIG%", config());
    else if (path.contains("%LOG%", Qt::CaseInsensitive))
        path.replace("%LOG%", log());
    _registry.insert(type, path);
    return true;
}

bool PathRegistry::cache() {
    const auto _d = data();
    const auto _c = config();
    const auto _l = log();
    return !_d.isEmpty() && !_c.isEmpty() && !_l.isEmpty();
}

bool PathRegistry::make(const QString& path) const {
    return QDir().mkpath(path);
}

QString UserOnlyPathRegistry::data() const {
    if (!_cachedData.isEmpty())
        return _cachedData;
    auto locations =
        QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);
    if (locations.isEmpty())
        locations = QStandardPaths::standardLocations(
            QStandardPaths::GenericDataLocation);
#ifdef Q_OS_WIN
    if (locations.isEmpty())
        locations = QStandardPaths::standardLocations(
            QStandardPaths::AppConfigLocation);
#endif
    if (locations.isEmpty())
        return QString();
    QString path;
#ifdef Q_OS_WIN
    auto it = std::find_if(
        std::cbegin(locations), std::cend(locations), [](const QString& entry) {
            return entry.contains("AppData/Local", Qt::CaseInsensitive);
        });
    if (it == std::cend(locations))
        it = std::cbegin(locations);
    path = (*it);
#endif
#ifdef Q_OS_MACOS
    auto it = std::find_if(
        std::cbegin(locations), std::cend(locations), [](const QString& entry) {
            return entry.startsWith("~/Library/Application Support",
                                    Qt::CaseInsensitive);
        });
    if (it == std::cend(locations))
        it = std::cbegin(locations);
    path = (*it);
#endif
#ifdef Q_OS_LINUX
    auto it = std::find_if(
        std::cbegin(locations), std::cend(locations), [](const QString& entry) {
            return entry.contains("/.local/share", Qt::CaseInsensitive);
        });
    if (it == std::cend(locations))
        it = std::cbegin(locations);
    path = (*it);
#endif
    if (!path.contains(QCoreApplication::applicationName(),
                       Qt::CaseInsensitive)) {
        if (!path.endsWith(QDir().separator()))
            path.append(QDir().separator());
        path.append(QCoreApplication::applicationName());
    }
    if (!make(path))
        return QString();
    _cachedData = path;
    return path;
}

QString UserOnlyPathRegistry::config() const {
    if (!_cachedConfig.isEmpty())
        return _cachedConfig;
    auto locations =
        QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    if (locations.isEmpty())
        locations =
            QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    if (locations.isEmpty())
        return QString();
    QString path;
#ifdef Q_OS_WIN
    auto it = std::find_if(
        std::cbegin(locations), std::cend(locations), [](const QString& entry) {
            return entry.contains("AppData/Local", Qt::CaseInsensitive);
        });
    if (it == std::cend(locations))
        it = std::cbegin(locations);
    path = (*it);
#endif
#ifdef Q_OS_MACOS
    path = locations[0];
#endif
#ifdef Q_OS_LINUX
    auto it = std::find_if(
        std::cbegin(locations), std::cend(locations), [](const QString& entry) {
            return entry.contains(".config", Qt::CaseInsensitive);
        });
    if (it == std::cend(locations))
        it = std::cbegin(locations);
    path = (*it);
#endif
    if (!path.contains(QCoreApplication::applicationName(),
                       Qt::CaseInsensitive)) {
        if (!path.endsWith(QDir().separator()))
            path.append(QDir().separator());
        path.append(QCoreApplication::applicationName());
    }
    if (!make(path))
        return QString();
    _cachedConfig = path;
    return path;
}

QString UserOnlyPathRegistry::log() const {
    if (!_cachedLog.isEmpty())
        return _cachedLog;
    QString path =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            .append("/log");
    if (!make(path))
        return QString();
    _cachedLog = path;
    return path;
}

QString SystemPathRegistry::data() const {
    if (!_cachedData.isEmpty())
        return _cachedData;
    auto locations =
        QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    if (locations.isEmpty())
        locations = QStandardPaths::standardLocations(
            QStandardPaths::AppLocalDataLocation);
#ifdef Q_OS_WIN
    if (locations.isEmpty())
        locations = QStandardPaths::standardLocations(
            QStandardPaths::AppConfigLocation);
#endif
    if (locations.isEmpty())
        return QString();
    QString path;
#ifdef Q_OS_WIN
    auto it = std::find_if(
        std::cbegin(locations), std::cend(locations), [](const QString& entry) {
            return entry.contains("ProgramData", Qt::CaseInsensitive);
        });
    if (it == std::cend(locations))
        it = std::cbegin(locations);
    path = (*it);
#endif
#ifdef Q_OS_MACOS
    auto it = std::find_if(
        std::cbegin(locations), std::cend(locations), [](const QString& entry) {
            return entry.startsWith("/Library/Application Support",
                                    Qt::CaseInsensitive);
        });
    if (it == std::cend(locations))
        it = std::cbegin(locations);
    path = (*it);
#endif
#ifdef Q_OS_LINUX
    auto it = std::find_if(
        std::cbegin(locations), std::cend(locations), [](const QString& entry) {
            return entry.startsWith("/usr/local/share", Qt::CaseInsensitive);
        });
    if (it == std::cend(locations))
        it = std::cbegin(locations);
    path = (*it);
#endif
    if (!path.contains(QCoreApplication::applicationName(),
                       Qt::CaseInsensitive)) {
        if (!path.endsWith(QDir().separator()))
            path.append(QDir().separator());
        path.append(QCoreApplication::applicationName());
    }
    if (!make(path))
        return QString();
    _cachedData = path;
    return path;
}

QString SystemPathRegistry::config() const {
    if (!_cachedConfig.isEmpty())
        return _cachedConfig;
    auto locations =
        QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    if (locations.isEmpty())
        locations =
            QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    if (locations.isEmpty())
        return QString();
    QString path;
#ifdef Q_OS_WIN
    auto it = std::find_if(
        std::cbegin(locations), std::cend(locations), [](const QString& entry) {
            return entry.contains("AppData/Local", Qt::CaseInsensitive);
        });
    if (it == std::cend(locations))
        it = std::cbegin(locations);
    path = (*it);
#endif
#ifdef Q_OS_MACOS
    path = locations[0];
#endif
#ifdef Q_OS_LINUX
    auto it = std::find_if(
        std::cbegin(locations), std::cend(locations), [](const QString& entry) {
            return entry.contains("xdg", Qt::CaseInsensitive);
        });
    if (it == std::cend(locations))
        it = std::cbegin(locations);
    path = (*it);
#endif
    if (!path.contains(QCoreApplication::applicationName(),
                       Qt::CaseInsensitive)) {
        if (!path.endsWith(QDir().separator()))
            path.append(QDir().separator());
        path.append(QCoreApplication::applicationName());
    }
    if (!make(path))
        return QString();
    _cachedConfig = path;
    return path;
}

QString SystemPathRegistry::log() const {
    if (!_cachedLog.isEmpty())
        return _cachedLog;
    QString path =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            .append("/log");
    if (!make(path))
        return QString();
    _cachedLog = path;
    return path;
}
