#include "uuidgeneratormodule.hpp"

#include <chrono>
#include <ctime>

#include <QDebug>
#include <QNetworkInterface>
#include <QRandomGenerator>
#include <QUuid>

#include "uuidgeneratormodulewidget.hpp"

#include "external/stduuid/uuid.h"

// Additional 100ns intervals from 1582 as chrono utc clock only counts from
// Already divided by 100 to calculate tick 100 ns tick count
constexpr auto SECOND_TICKS_FROM_1582_TO_EPOCH
    = std::chrono::seconds(12219292800 / 100);

UUIDGeneratorModule::UUIDGeneratorModule(const Dependency &dependency,
                                         QObject *parent)
    : Module(tr("UUID Generator"), "com.dontsoft.devtools.uuidgenerator",
             QObject::tr("Generators"), QIcon(), parent),
      Dependant<QSharedPointer<ConfigEngine>>(dependency),
      Loggable("com.dontsoft.devtools.uuidgenerator"),
      _widget(new UUIDGeneratorModuleWidget({tr("UUID v1"), tr("UUID v3"),
                                             tr("UUID v4"), tr("UUID v5"),
                                             tr("System Generator")}))
{

    auto widgetCasted = static_cast<UUIDGeneratorModuleWidget *>(_widget);

    connect(widgetCasted, &UUIDGeneratorModuleWidget::v1NetworkInterfaceChanged,
            this, &UUIDGeneratorModule::v1NetworkInterfaceChanged);
    connect(widgetCasted,
            &UUIDGeneratorModuleWidget::v1useRandomClockSequenceChanged, this,
            &UUIDGeneratorModule::v1useRandomClockSequenceChanged);

    connect(widgetCasted, &UUIDGeneratorModuleWidget::namespaceUpdated, this,
            &UUIDGeneratorModule::namespaceUpdated);
    connect(widgetCasted, &UUIDGeneratorModuleWidget::baseDataUpdated, this,
            &UUIDGeneratorModule::baseDataUpdated);

    connect(widgetCasted, &UUIDGeneratorModuleWidget::generate, this,
            &UUIDGeneratorModule::generate);

    connect(this, &UUIDGeneratorModule::newUUIDsGenerated, widgetCasted,
            &UUIDGeneratorModuleWidget::generatedNewUUIDs);

    widgetCasted->triggerAllConfigSignals();
}

QWidget *UUIDGeneratorModule::getWidget() const
{
    return _widget;
}

quint64 UUIDGeneratorModule::generateTimestamp() const
{
    const auto now = std::chrono::system_clock::now();
    return std::chrono::nanoseconds(now.time_since_epoch()).count() / 100
           + std::chrono::nanoseconds(SECOND_TICKS_FROM_1582_TO_EPOCH).count();
}

quint16 UUIDGeneratorModule::generateV1ClockSequence() const
{
    if (_v1UseRandomState)
    {
        return QRandomGenerator::system()->bounded(
            0, std::numeric_limits<quint16>::max());
    }
    auto configEngine = getDependency<0>();
    auto clockSequence
        = configEngine->get<quint16>(getIdentifier() + "/V1ClockSequence", 0);
    configEngine->set(getIdentifier() + "/V1ClockSequence", clockSequence + 1);
    return clockSequence;
}

QString UUIDGeneratorModule::generateRandomMacAddress() const
{
    return QString("%1:%2:%3:%4:%5:%6")
        .arg(static_cast<quint8>(QRandomGenerator::system()->bounded(
                                     0, std::numeric_limits<quint8>::max()),
                                 0, 16))
        .arg(static_cast<quint8>(QRandomGenerator::system()->bounded(
                                     0, std::numeric_limits<quint8>::max()),
                                 0, 16))
        .arg(static_cast<quint8>(QRandomGenerator::system()->bounded(
                                     0, std::numeric_limits<quint8>::max()),
                                 0, 16))
        .arg(static_cast<quint8>(QRandomGenerator::system()->bounded(
                                     0, std::numeric_limits<quint8>::max()),
                                 0, 16))
        .arg(static_cast<quint8>(QRandomGenerator::system()->bounded(
                                     0, std::numeric_limits<quint8>::max()),
                                 0, 16))
        .arg(static_cast<quint8>(QRandomGenerator::system()->bounded(
                                     0, std::numeric_limits<quint8>::max()),
                                 0, 16));
}

constexpr quint8 VERSION_V1 = 0b1000;
constexpr quint8 VARIANT = 0b100;

quint16 UUIDGeneratorModule::padTimestampWithVersion(quint16 timeHigh,
                                                     int version) const
{

    return timeHigh | (VERSION_V1 << 12);
}

void UUIDGeneratorModule::generate(int version, int count)
{
    QList<QUuid> generatedIds;
    for (int i = 0; i < count; ++i)
    {
        switch (version)
        {
        case 0:
            generatedIds.append(regenerateV1());
            break;
        case 1:
            generatedIds.append(regenerateV3());
            break;
        case 2:
            generatedIds.append(regenerateV4());
            break;
        case 3:
            generatedIds.append(regenerateV5());
            break;
        case 4:
            generatedIds.append(regenerateSystem());
            break;
        case 5:
            generatedIds.append(regenerateTime());
            break;
        }
    }
    emit newUUIDsGenerated(generatedIds);
}

void UUIDGeneratorModule::v1NetworkInterfaceChanged(
    const QString &networkInterfaceName, const QString &hardwareAddress)
{
    _v1HardwareInterfaceName = networkInterfaceName;
    _v1HardwareAddress = hardwareAddress;
}

void UUIDGeneratorModule::v1useRandomClockSequenceChanged(bool useRandomState)
{
    _v1UseRandomState = useRandomState;
}

void UUIDGeneratorModule::namespaceUpdated(const QUuid &uuidNamespace)
{
    _namespace = uuidNamespace;
}

void UUIDGeneratorModule::baseDataUpdated(const QString &baseData)
{
    _baseData = baseData;
}

QUuid UUIDGeneratorModule::regenerateV1() const
{
    const quint64 timestamp = generateTimestamp();
    const quint32 timeLow = timestamp & 0xFFFFFFFF;
    const quint16 timeMid = (timestamp >> 32) & 0xFFFF;
    const quint16 timeHigh = ((timestamp >> 48) & 0x0FFF) | (VERSION_V1 << 9);
    const quint16 clockSequence = generateV1ClockSequence();
    const quint8 clockSequenceLow = clockSequence & 0xFF;
    const quint8 clockSequenceHigh
        = ((clockSequence >> 8) & 0x3F) | (VARIANT << 5);

    QString hardwareAddress = _v1HardwareAddress;

    if (hardwareAddress.isEmpty() || hardwareAddress.count(":") < 5)
    {
        hardwareAddress = generateRandomMacAddress();
    }
    const QStringList splittedAddress = hardwareAddress.split(":");
    quint8 b1 = static_cast<quint8>(splittedAddress[0].toInt(nullptr, 16));
    quint8 b2 = static_cast<quint8>(splittedAddress[1].toInt(nullptr, 16));
    quint8 b3 = static_cast<quint8>(splittedAddress[2].toInt(nullptr, 16));
    quint8 b4 = static_cast<quint8>(splittedAddress[3].toInt(nullptr, 16));
    quint8 b5 = static_cast<quint8>(splittedAddress[4].toInt(nullptr, 16));
    quint8 b6 = static_cast<quint8>(splittedAddress[5].toInt(nullptr, 16));

    return QUuid(timeLow, timeMid, timeHigh, clockSequenceHigh,
                 clockSequenceLow, b1, b2, b3, b4, b5, b6);
}

QUuid UUIDGeneratorModule::regenerateV2() const
{
    return QUuid();
}

QUuid UUIDGeneratorModule::regenerateV3() const
{
    return QUuid::createUuidV3(_namespace, _baseData);
}

QUuid UUIDGeneratorModule::regenerateV4() const
{
    return QUuid::createUuid();
}

QUuid UUIDGeneratorModule::regenerateV5() const
{
    return QUuid::createUuidV5(_namespace, _baseData);
}

QUuid UUIDGeneratorModule::regenerateTime() const
{
    uuids::uuid_time_generator gen{};
    return QUuid::fromString(QString::fromStdString(uuids::to_string(gen())));
}

QUuid UUIDGeneratorModule::regenerateSystem() const
{
    uuids::uuid_system_generator gen{};
    return QUuid::fromString(QString::fromStdString(uuids::to_string(gen())));
}
