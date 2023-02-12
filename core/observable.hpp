#ifndef OBSERVABLE_HPP
#define OBSERVABLE_HPP

#include <QMutex>
#include <QMutexLocker>
#include <QSharedPointer>
#include <QVector>

#include "observer.hpp"

template <class C, typename ParameterType>
class Observable {
   public:
    void notify(C* source, const QString& key, const ParameterType& params) {
        QMutexLocker locker(&_mutex);
        auto observers = _observers;
        locker.unlock();
        for (auto observer : observers) {
            if (observer)
                observer->notificationReceived(source, key, params);
        }
    }

    void subscribe(Observer<C, ParameterType>* observer) {
        QMutexLocker locker(&_mutex);
        _observers.append(observer);
    }

    void unsubscribe(Observer<C, ParameterType>* observer) {
        QMutexLocker locker(&_mutex);
        _observers.removeAll(observer);
    }

   private:
    QMutex _mutex;
    QVector<Observer<C, ParameterType>*> _observers;
};

#endif
