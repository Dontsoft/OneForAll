#include "configengine.hpp"

#include <chrono>

#include <QMutexLocker>
#include <QTimer>

using namespace std::chrono_literals;

constexpr auto FLUSH_TIMER_INTERVAL = 100ms;

ConfigEngine::ConfigEngine(const FlushPolicy& flushPolicy)
    : _flushPolicy(flushPolicy),
      _flushTimer(QPointer<QTimer>(new QTimer(this))) {
    if (_flushPolicy.type ==
        ConfigEngine::FlushPolicy::Type::QueuedAfterChange) {
        connect(_flushTimer.data(), &QTimer::timeout, this,
                &ConfigEngine::queuedFlush);
        _flushTimer->setSingleShot(true);
        _flushTimer->setInterval(FLUSH_TIMER_INTERVAL);
    }
}

void ConfigEngine::load() {
    QMutexLocker locker(&_mutex);
    _cache = getAll();
}

void ConfigEngine::set(const QString& key, const QVariant& value) {
    QVariant oldValue;
    QMutexLocker locker(&_mutex);
    if (!_cache.contains(key)) {
        _dirty = true;
    } else if (_cache.value(key) != value) {
        oldValue = _cache.value(key);
        _dirty = true;
    }
    _cache[key] = value;
    bool dirty = _dirty;
    locker.unlock();
    if (dirty) {
        bool shouldFlush = false;
        switch (_flushPolicy.type) {
            case ConfigEngine::FlushPolicy::Type::AfterChange:
                shouldFlush = true;
                break;
            case ConfigEngine::FlushPolicy::Type::QueuedAfterChange:
                shouldFlush = false;
                _flushTimer->start();
                break;
            case ConfigEngine::FlushPolicy::Type::ManualFlush:
                shouldFlush = false;
                break;
        }
        if (shouldFlush) {
            locker.relock();
            auto cache = _cache;
            locker.unlock();
            flushImplementation(cache);
        }
        notify(this, key, qMakePair(value, oldValue));
        emit changed(key, value, oldValue);
    }
}

QVariant ConfigEngine::get(const QString& key,
                           const QVariant& defaultValue) const {
    QMutexLocker locker(&_mutex);
    return _cache.value(key, defaultValue);
}

void ConfigEngine::flush() { queuedFlush(); }

void ConfigEngine::queuedFlush() {
    QMutexLocker locker(&_mutex);
    if (!_dirty)
        return;
    auto cache = _cache;
    locker.unlock();
    flushImplementation(cache);
}
