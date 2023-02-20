#include "uuidgeneratormodulewidget.hpp"
#include "ui_uuidgeneratormodulewidget.h"

#include <chrono>
#include <ctime>

#include <QClipboard>
#include <QDebug>
#include <QNetworkInterface>
#include <QRegularExpressionValidator>
#include <QUuid>

#include "external/stduuid/uuid.h"

// Additional 100ns intervals from 1582 as chrono utc clock only counts from
// Already divided by 100 to calculate tick 100 ns tick count
constexpr auto SECOND_TICKS_FROM_1582_TO_EPOCH
    = std::chrono::seconds(12218428800 / 100);

QUuid generate_v1()
{
}

UUIDGeneratorModuleWidget::UUIDGeneratorModuleWidget(
    const QList<QString>& versionList, QWidget* parent)
    : QWidget(parent), ui(new Ui::UUIDGeneratorModuleWidget)
{
    ui->setupUi(this);
    connect(ui->regenerateButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::regenerate);
    connect(ui->copyOutputButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::copyOutput);
    connect(ui->clearOutputButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::clearOutput);
    connect(ui->moreOutputOptionsButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::togglOutputOptions);
    togglOutputOptions(false);

    ui->uuidGenerationSelectionComboBox->addItems(versionList);

    for (const auto& interface : QNetworkInterface::allInterfaces())
    {
        if (!interface.hardwareAddress().isEmpty())
        {
            ui->uuidV1NetworkInterfaceComboBox->addItem(
                interface.humanReadableName() + " ("
                    + interface.hardwareAddress() + ")",
                interface.hardwareAddress());
        }
    }
    ui->uuidV1NetworkInterfaceComboBox->addItem(tr("Use Random Node"));
    ui->parameterWidget->setCurrentIndex(0);

    ui->v3NamespaceEdit->setText(QUuid().toString());
    ui->v5NamespaceEdit->setText(QUuid().toString());
    ui->v3NamespaceEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("^\\{{0,1}[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{"
                           "4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}\\}{0,1}$")));
    ui->v5NamespaceEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("^\\{{0,1}[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{"
                           "4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}\\}{0,1}$")));

    connect(ui->uuidGenerationSelectionComboBox,
            &QComboBox::currentIndexChanged, ui->parameterWidget,
            &QStackedWidget::setCurrentIndex);

    connect(ui->uuidGenerationSelectionComboBox,
            &QComboBox::currentIndexChanged, this,
            &UUIDGeneratorModuleWidget::regenerateVersionChange);

    connect(ui->uuidV1NetworkInterfaceComboBox, &QComboBox::currentIndexChanged,
            this,
            &UUIDGeneratorModuleWidget::v1networkInterfaceCurrentIndexChanged);
    connect(ui->uuidV1RandomClockSequenceCheckBox, &QCheckBox::stateChanged,
            this,
            &UUIDGeneratorModuleWidget::v1useRandomClockSequenceStateChanged);

    connect(ui->v3DNSNamespaceButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::generateDNSNamespace);
    connect(ui->v3URLNamespaceButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::generateURLNamespace);
    connect(ui->v3OIDNamespaceButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::generateOIDNamespace);
    connect(ui->v3X500DNNamespaceButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::generateX500DNNamespace);
    connect(ui->v3RandomNamespaceButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::generateRandomNamespace);

    connect(ui->v5DNSNamespaceButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::generateDNSNamespace);
    connect(ui->v5URLNamespaceButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::generateURLNamespace);
    connect(ui->v5OIDNamespaceButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::generateOIDNamespace);
    connect(ui->v5X500DNNamespaceButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::generateX500DNNamespace);
    connect(ui->v5RandomNamespaceButton, &QPushButton::clicked, this,
            &UUIDGeneratorModuleWidget::generateRandomNamespace);

    connect(ui->v3BaseDataEdit, &QLineEdit::textEdited, this,
            &UUIDGeneratorModuleWidget::v3BaseDataEdited);
    connect(ui->v5BaseDataEdit, &QLineEdit::textEdited, this,
            &UUIDGeneratorModuleWidget::v5BaseDataEdited);
    connect(ui->v3NamespaceEdit, &QLineEdit::textEdited, this,
            &UUIDGeneratorModuleWidget::v3NamespaceEdited);
    connect(ui->v5NamespaceEdit, &QLineEdit::textEdited, this,
            &UUIDGeneratorModuleWidget::v5NamespaceEdited);
}

void UUIDGeneratorModuleWidget::triggerAllConfigSignals()
{
    emit v1networkInterfaceCurrentIndexChanged(
        ui->uuidV1NetworkInterfaceComboBox->currentIndex());
    emit v1useRandomClockSequenceStateChanged(
        ui->uuidV1RandomClockSequenceCheckBox->checkState());
    emit namespaceUpdated(QUuid::fromString(ui->v3NamespaceEdit->text()));
    emit baseDataUpdated(ui->v3BaseDataEdit->text());
}

UUIDGeneratorModuleWidget::~UUIDGeneratorModuleWidget()
{
    delete ui;
}

void UUIDGeneratorModuleWidget::togglOutputOptions(bool enabled)
{
    ui->moreOutputOptionsButton->setText(enabled
                                             ? tr("Less Output Options <<<")
                                             : tr("More Output Options >>>"));
    ui->outputOptionsWidget->setVisible(enabled);
}

void UUIDGeneratorModuleWidget::regenerate()
{
    regenerateVersionChange(
        ui->uuidGenerationSelectionComboBox->currentIndex());
}

void UUIDGeneratorModuleWidget::copyOutput()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->outputEdit->toPlainText());
}

void UUIDGeneratorModuleWidget::clearOutput()
{
    ui->outputEdit->clear();
}

void UUIDGeneratorModuleWidget::generatedNewUUIDs(const QList<QUuid>& uuids)
{
    ui->outputEdit->clear();
    QList<QString> uuidStringList;
    for (const auto& uuid : uuids)
    {
        uuidStringList.append(uuid.toString(static_cast<QUuid::StringFormat>(
            ui->outputTypeComboBox->currentIndex())));
    }
    auto text = uuidStringList.join(QChar(QChar::LineFeed));
    if (ui->outputUppercaseCheckBox->isChecked())
    {
        text = text.toUpper();
    }
    ui->outputEdit->setPlainText(text);
}

void UUIDGeneratorModuleWidget::generateDNSNamespace()
{
    updateNamespace(
        QUuid::fromString("{6ba7b810-9dad-11d1-80b4-00c04fd430c8}"));
}

void UUIDGeneratorModuleWidget::generateURLNamespace()
{
    updateNamespace(
        QUuid::fromString("{6ba7b811-9dad-11d1-80b4-00c04fd430c8}"));
}

void UUIDGeneratorModuleWidget::generateOIDNamespace()
{
    updateNamespace(
        QUuid::fromString("{6ba7b812-9dad-11d1-80b4-00c04fd430c8}"));
}

void UUIDGeneratorModuleWidget::generateX500DNNamespace()
{
    updateNamespace(
        QUuid::fromString("{6ba7b814-9dad-11d1-80b4-00c04fd430c8}"));
}

void UUIDGeneratorModuleWidget::generateRandomNamespace()
{
    updateNamespace(QUuid::createUuid());
}

void UUIDGeneratorModuleWidget::updateNamespace(const QUuid& uuid)
{
    ui->v3NamespaceEdit->setText(uuid.toString());
    ui->v5NamespaceEdit->setText(uuid.toString());
    emit namespaceUpdated(uuid);
}

void UUIDGeneratorModuleWidget::regenerateVersionChange(int index)
{
    emit generate(index, ui->outputCountSpinBox->value());
}

void UUIDGeneratorModuleWidget::v1networkInterfaceCurrentIndexChanged(int index)
{
    const auto networkInterfaceName
        = ui->uuidV1NetworkInterfaceComboBox->currentText();
    const auto hardwareAddress = qvariant_cast<QString>(
        ui->uuidV1NetworkInterfaceComboBox->currentData());
    emit v1NetworkInterfaceChanged(networkInterfaceName, hardwareAddress);
}

void UUIDGeneratorModuleWidget::v1useRandomClockSequenceStateChanged(int state)
{
    emit v1useRandomClockSequenceChanged(state == Qt::Checked);
}

void UUIDGeneratorModuleWidget::v3BaseDataEdited(const QString& text)
{
    ui->v5BaseDataEdit->setText(text);
    emit baseDataUpdated(text);
}

void UUIDGeneratorModuleWidget::v3NamespaceEdited(const QString& text)
{
    ui->v5NamespaceEdit->setText(text);
    emit namespaceUpdated(QUuid::fromString(text));
}

void UUIDGeneratorModuleWidget::v5BaseDataEdited(const QString& text)
{
    ui->v3BaseDataEdit->setText(text);
    emit baseDataUpdated(text);
}

void UUIDGeneratorModuleWidget::v5NamespaceEdited(const QString& text)
{
    ui->v3NamespaceEdit->setText(text);
    emit namespaceUpdated(QUuid::fromString(text));
}
