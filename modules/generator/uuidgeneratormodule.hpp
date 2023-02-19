#ifndef UUIDGENERATORMODULE_HPP
#define UUIDGENERATORMODULE_HPP
#include "modules/module.hpp"

#include <QList>
#include <QObject>
#include <QUuid>

#include "config/configengine.hpp"
#include "core/dependant.hpp"
#include "log/loggable.hpp"

class UUIDGeneratorModule : public Module,
                            public Dependant<QSharedPointer<ConfigEngine>>,
                            public Loggable
{
    Q_OBJECT
public:
    explicit UUIDGeneratorModule(
        const UUIDGeneratorModule::Dependency& dependency,
        QObject* parent = nullptr);
    QWidget* getWidget() const override;

public slots:
    void generate(int version, int count);

    void v1NetworkInterfaceChanged(const QString& networkInterfaceName,
                                   const QString& hardwareAddress);
    void v1useRandomClockSequenceChanged(bool useRandomState);

    void namespaceUpdated(const QUuid& uuidNamespace);
    void baseDataUpdated(const QString& baseData);

signals:
    void newUUIDsGenerated(const QList<QUuid>& uuids);

private:
    quint64 generateTimestamp() const;
    quint16 generateV1ClockSequence() const;
    QString generateRandomMacAddress() const;
    quint16 padTimestampWithVersion(quint16 timeHigh, int version) const;

    QUuid regenerateV1() const;
    QUuid regenerateV2() const;
    QUuid regenerateV3() const;
    QUuid regenerateV4() const;
    QUuid regenerateV5() const;
    QUuid regenerateTime() const;
    QUuid regenerateSystem() const;

    QWidget* _widget;
    QString _v1HardwareInterfaceName;
    QString _v1HardwareAddress;
    bool _v1UseRandomState;
    QUuid _namespace;
    QString _baseData;
};

#endif // UUIDGENERATORMODULE_HPP
