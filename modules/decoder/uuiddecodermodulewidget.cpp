#include "uuiddecodermodulewidget.hpp"
#include "ui_uuiddecodermodulewidget.h"

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
}

UUIDDecoderModuleWidget::~UUIDDecoderModuleWidget()
{
    delete ui;
}

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
        id.toString(QUuid::WithoutBraces).toUpper());
    ui->upperCaseWithBracketsEdit->setText(id.toString().toLower());
    ui->upperCaseWithoutBracketsEdit->setText(
        id.toString(QUuid::WithoutBraces).toUpper());

    const auto byteArray = id.toByteArray(QUuid::WithoutBraces);
}
