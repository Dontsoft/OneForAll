#include "uuiddecodermodulewidget.hpp"
#include "ui_uuiddecodermodulewidget.h"

#include <QDateTime>
#include <QRegularExpressionValidator>
#include <QUuid>

UUIDDecoderModuleWidget::UUIDDecoderModuleWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::UUIDDecoderModuleWidget)
{
    ui->setupUi(this);
    ui->inputEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("^\\{{0,1}[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{"
                           "4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}\\}{0,1}$")));

    connect(ui->decodeButton, &QPushButton::clicked, this,
            &UUIDDecoderModuleWidget::decode);
    connect(ui->inputEdit, &QLineEdit::editingFinished, this,
            &UUIDDecoderModuleWidget::decode);
    decode();
}

UUIDDecoderModuleWidget::~UUIDDecoderModuleWidget()
{
    delete ui;
}

constexpr quint8 VERSION_V1 = 0b1000;
constexpr quint8 VARIANT = 0b100;
constexpr auto SECOND_TICKS_FROM_1582_TO_EPOCH
    = std::chrono::seconds(12219292800 / 100);

void UUIDDecoderModuleWidget::decode()
{
    const auto input = ui->inputEdit->text();
    auto replacedInput = input;
    replacedInput = replacedInput.remove("{").remove("}").remove("-");
    const bool isNil = replacedInput.count("0") == replacedInput.length();
    const QUuid id = QUuid::fromString(input);
    const bool isError
        = (!isNil && id.isNull()) || replacedInput.length() != 32;
    ui->validEdit->setText(isError ? tr("Not valid") : tr("Valid"));

    if (!isError & id.isNull())
    {
        ui->versionEdit->setText(tr("NIL UUID"));
        ui->variantEdit->setText(tr("NIL UUID"));
    }
    else
    {

        switch (id.version())
        {
        case QUuid::VerUnknown:
            ui->versionEdit->setText(tr("Unknown Version"));
            break;
        case QUuid::Time:
            ui->versionEdit->setText(tr("UUID v1 (Time)"));
            break;
        case QUuid::EmbeddedPOSIX:
            ui->versionEdit->setText(tr("UUID v2 (DCE Security Version)"));
            break;
        case QUuid::Md5:
            ui->versionEdit->setText(tr("UUID v3 (MD5)"));
            break;
        case QUuid::Random:
            ui->versionEdit->setText(tr("UUID v4 (Random)"));
            break;
        case QUuid::Sha1:
            ui->versionEdit->setText(tr("UUID v5 (SHA1)"));
            break;
        }

        switch (id.variant())
        {
        case QUuid::VarUnknown:
            ui->variantEdit->setText(tr("Unknown Variant"));
            break;
        case QUuid::NCS:
            ui->variantEdit->setText(tr("NCS (Network Computing System)"));
            break;
        case QUuid::DCE:
            ui->variantEdit->setText(
                tr("DCE (Distributed Computing Environment)"));
            break;
        case QUuid::Microsoft:
            ui->variantEdit->setText(tr("Microsoft (GUID)"));
            break;
        case QUuid::Reserved:
            ui->variantEdit->setText(tr("Reserved"));
            break;
        }
    }
    ui->lowerCaseWithBracketsEdit->setText(id.toString().toLower());
    ui->lowerCaseWithoutBracketsEdit->setText(
        id.toString(QUuid::WithoutBraces).toLower());

    ui->upperCaseWithBracketsEdit->setText(id.toString().toUpper());
    ui->upperCaseWithoutBracketsEdit->setText(
        id.toString(QUuid::WithoutBraces).toUpper());

    ui->rfc4122Edit->setText(id.toRfc4122().toHex(':'));

    ui->contentWidget->setCurrentIndex(
        id.version() != QUuid::Time || isError ? 0 : 1);
    if (id.version() != QUuid::Time)
    {
        ui->contentEdit->setText(id.toByteArray(QUuid::WithoutBraces));
    }
    else
    {
        const auto rfcByteArray = id.toRfc4122();
        const quint32 timeLow
            = quint8(rfcByteArray[0]) << 24 | quint8(rfcByteArray[1]) << 16
              | quint8(rfcByteArray[2]) << 8 | quint8(rfcByteArray[3]);
        const quint16 timeMid
            = quint8(rfcByteArray[4]) << 8 | quint8(rfcByteArray[5]);
        const quint16 timeHigh
            = (quint8(rfcByteArray[6]) << 8 | quint8(rfcByteArray[7]))
              & ~(VERSION_V1 << 9);
        const quint64 timestamp
            = ((((quint64(timeHigh) << 48) | (quint64(timeMid) << 32)
                 | quint64(timeLow))
                & 0x0FFFFFFFFFFFFFFF)
               - std::chrono::nanoseconds(SECOND_TICKS_FROM_1582_TO_EPOCH)
                     .count())
              * 100;

        const quint8 clockSequenceLow = quint8(rfcByteArray[8]);
        const quint8 clockSequenceHigh
            = quint8(rfcByteArray[9]) & ~(VARIANT << 5);
        const quint16 clockSequence
            = (clockSequenceHigh | (clockSequenceLow << 8)) & 0x3FFF;

        const QStringList splittedAddress{
            QString::number(quint8(rfcByteArray[10]), 16),
            QString::number(quint8(rfcByteArray[11]), 16),
            QString::number(quint8(rfcByteArray[12]), 16),
            QString::number(quint8(rfcByteArray[13]), 16),
            QString::number(quint8(rfcByteArray[14]), 16),
            QString::number(quint8(rfcByteArray[15]), 16)};

        const auto timestampNanoseconds = std::chrono::nanoseconds(timestamp);
        ui->clockEdit->setText(QString::number(clockSequence));
        ui->nodeEdit->setText(splittedAddress.join(":").toUpper());
        ui->timeEdit->setText(
            QDateTime::fromMSecsSinceEpoch(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    timestampNanoseconds)
                    .count())
                .toString());
    }
}
