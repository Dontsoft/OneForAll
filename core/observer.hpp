#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <QString>

template <class C, typename ParameterType>
class Observer {
   public:
    Observer() = default;
    virtual void notificationReceived(C* sender, const QString& key,
                                      const ParameterType& params) = 0;
};

#endif
