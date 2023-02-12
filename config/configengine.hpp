#ifndef CONFIG_ENGINE_HPP

#define CONFIG_ENGINE_HPP

#include <QHash>
#include <QMutex>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QVariant>

#include "core/observable.hpp"

class QTimer;

class ConfigEngine
    : public QObject,
      public Observable<ConfigEngine, QPair<QVariant, QVariant>> {

    Q_OBJECT

   public:
    struct FlushPolicy {

        enum class Type { AfterChange, QueuedAfterChange, ManualFlush } type;
    };

    explicit ConfigEngine(const FlushPolicy& flushPolicy);

    void load();

    void set(const QString& key, const QVariant& value);

    QVariant get(const QString& key,
                 const QVariant& defaultValue = QVariant()) const;

    template <typename T>
    inline T get(const QString& key, const T& defaultValue = T()) const {

        return qvariant_cast<T>(get(key, QVariant::fromValue(defaultValue)));
    }

    void flush();

   signals:

    void changed(const QString& key, const QVariant& newValue,
                 const QVariant& oldValue);

   protected:
    virtual void flushImplementation(
        const QHash<QString, QVariant>& values) = 0;

    virtual QHash<QString, QVariant> getAll() const = 0;

   private slots:

    void queuedFlush();

   private:
    const FlushPolicy _flushPolicy;

    mutable QMutex _mutex;

    mutable bool _dirty = false;

    QPointer<QTimer> _flushTimer;

    QHash<QString, QVariant> _cache;

#ifdef QT_TESTLIB_LIB

   public:
    QHash<QString, QVariant> getAllTest() { return getAll(); }

#endif
};

#endif
