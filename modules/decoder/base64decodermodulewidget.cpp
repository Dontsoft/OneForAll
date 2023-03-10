#include "base64decodermodulewidget.hpp"
#include "ui_base64decodermodulewidget.h"

#include <QClipboard>
#include <QFileDialog>
#include <QStandardPaths>

Base64DecoderModuleWidget::Base64DecoderModuleWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::Base64DecoderModuleWidget)
{
    ui->setupUi(this);

    connect(ui->clearInputButton, &QPushButton::clicked, this,
            &Base64DecoderModuleWidget::clearInput);
    connect(ui->clearOutputButton, &QPushButton::clicked, this,
            &Base64DecoderModuleWidget::clearOutput);

    connect(ui->inputEdit, &QPlainTextEdit::textChanged, this,
            &Base64DecoderModuleWidget::decodeButtonClicked);
    connect(ui->fileUploadWidget, &FileUploadWidget::fileChanged, this,
            &Base64DecoderModuleWidget::filenameChanged);

    connect(ui->decodeButton, &QPushButton::clicked, this,
            &Base64DecoderModuleWidget::decodeButtonClicked);
    connect(ui->saveOutputAsFileButton, &QPushButton::clicked, this,
            &Base64DecoderModuleWidget::saveAsFile);
    connect(ui->copyOutputButton, &QPushButton::clicked, this,
            &Base64DecoderModuleWidget::copyOutput);
}

Base64DecoderModuleWidget::~Base64DecoderModuleWidget()
{
    delete ui;
}
void Base64DecoderModuleWidget::clearInput()
{
    ui->inputEdit->clear();
}
void Base64DecoderModuleWidget::clearOutput()
{
    ui->outputEdit->clear();
}

void Base64DecoderModuleWidget::copyOutput()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->outputEdit->toPlainText());
}

void Base64DecoderModuleWidget::filenameChanged(const QString& filename)
{
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->inputEdit->setPlainText(f.readAll());
    }
}

void Base64DecoderModuleWidget::decodeButtonClicked()
{
    decode(ui->inputEdit->toPlainText());
}

void Base64DecoderModuleWidget::decoded(const QByteArray& byteArray)
{
    _output = byteArray;
    ui->outputEdit->setPlainText(byteArray);
}

void Base64DecoderModuleWidget::saveAsFile()
{
    const QString filename = QFileDialog::getSaveFileName(
        nullptr, tr("Save encoded content"),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        QString(), nullptr, QFileDialog::DontUseNativeDialog);
    if (filename.isEmpty())
    {
        return;
    }
    QFile f(filename);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        f.write(_output);
    }
}
