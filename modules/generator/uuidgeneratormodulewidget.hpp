#ifndef UUIDGENERATORMODULEWIDGET_HPP
#define UUIDGENERATORMODULEWIDGET_HPP

#include <functional>

#include <QUuid>
#include <QWidget>

namespace Ui {
    class UUIDGeneratorModuleWidget;
}

class UUIDGeneratorModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UUIDGeneratorModuleWidget(const QList<QString> &versionList,
                                       QWidget *parent = nullptr);
    ~UUIDGeneratorModuleWidget();

    void triggerAllConfigSignals();

signals:
    void generate(int version, int count);
    void v1NetworkInterfaceChanged(const QString &networkInterfaceName,
                                   const QString &hardwareAddress);
    void v1useRandomClockSequenceChanged(bool useRandom);

    void namespaceUpdated(const QUuid &uuidNamespace);
    void baseDataUpdated(const QString &baseData);

public slots:
    void togglOutputOptions(bool enabled);

    void regenerate();
    void copyOutput();
    void clearOutput();
    void regenerateVersionChange(int index);
    void generatedNewUUIDs(const QList<QUuid> &uuids);

    void generateDNSNamespace();
    void generateURLNamespace();
    void generateOIDNamespace();
    void generateX500DNNamespace();
    void generateRandomNamespace();

    void updateNamespace(const QUuid &uuid);

private slots:
    void v1networkInterfaceCurrentIndexChanged(int index);
    void v1useRandomClockSequenceStateChanged(int state);

    void v3BaseDataEdited(const QString &text);
    void v3NamespaceEdited(const QString &text);
    void v5BaseDataEdited(const QString &text);
    void v5NamespaceEdited(const QString &text);

private:
    Ui::UUIDGeneratorModuleWidget *ui;
};

#endif // UUIDGENERATORMODULEWIDGET_HPP
