#include "base64encodermodulewidget.hpp"
#include "ui_base64encodermodulewidget.h"

#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>

Base64EncoderModuleWidget::Base64EncoderModuleWidget(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::Base64EncoderModuleWidget)
{
    ui->setupUi(this);
    connect(ui->encodingModeComboBox, &QComboBox::currentIndexChanged,
            ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    connect(ui->fileUploadWidget, &FileUploadWidget::fileChanged, this,
            &Base64EncoderModuleWidget::fileChanged);

    connect(ui->selectFileButton, &QPushButton::clicked, this,
            &Base64EncoderModuleWidget::selectFile);
    connect(ui->encodeButton, &QPushButton::clicked, this,
            &Base64EncoderModuleWidget::encodeFileContent);

    connect(ui->textEdit, &QPlainTextEdit::textChanged, this,
            &Base64EncoderModuleWidget::textChanged);

    connect(ui->saveAsFileButton, &QPushButton::clicked, this,
            &Base64EncoderModuleWidget::saveEncodedAsFiled);

    connect(ui->base64OptionsComboBox, &QComboBox::currentIndexChanged, this,
            &Base64EncoderModuleWidget::base64OptionsComboBoxChanged);

    ui->encodingModeComboBox->setCurrentIndex(0);
}

Base64EncoderModuleWidget::~Base64EncoderModuleWidget()
{
    delete ui;
}

void Base64EncoderModuleWidget::base64OptionsComboBoxChanged(int index)
{
    QByteArray::Base64Options options;
    switch (index)
    {
    case 0:
        options.setFlag(QByteArray::Base64Encoding);
        options.setFlag(QByteArray::KeepTrailingEquals);
        break;
    case 1:
        options.setFlag(QByteArray::Base64Encoding);
        options.setFlag(QByteArray::OmitTrailingEquals);
        break;
    case 2:
        options.setFlag(QByteArray::Base64UrlEncoding);
        options.setFlag(QByteArray::KeepTrailingEquals);
        break;
    case 3:
        options.setFlag(QByteArray::Base64UrlEncoding);
        options.setFlag(QByteArray::OmitTrailingEquals);
        break;
    }
    emit base64OptionsChanged(options);
    if (ui->encodingModeComboBox->currentIndex() == 0)
    {
        textChanged();
    }
    else
    {
        encodeFileContent();
    }
}

void Base64EncoderModuleWidget::selectFile()
{
    const QString fileName = QFileDialog::getOpenFileName(
        nullptr, tr("Select file ..."),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        QString(), nullptr, QFileDialog::DontUseNativeDialog);
    if (fileName.isEmpty())
    {
        return;
    }
    fileChanged(fileName);
}

void Base64EncoderModuleWidget::fileChanged(const QString& filename)
{
    ui->filePathEdit->setText(filename);
}

void Base64EncoderModuleWidget::encodeFileContent()
{
    QFile f(ui->filePathEdit->text());
    if (f.open(QIODevice::ReadOnly))
    {
        emit encode(f.readAll());
    }
}

void Base64EncoderModuleWidget::textChanged()
{
    const auto text = ui->textEdit->toPlainText();
    emit encode(text.toUtf8());
}

void Base64EncoderModuleWidget::encoded(const QString& encodedContent)
{
    ui->encodedEdit->setPlainText(encodedContent);
}

void Base64EncoderModuleWidget::saveEncodedAsFiled()
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
        f.write(ui->encodedEdit->toPlainText().toUtf8());
    }
}

void Base64EncoderModuleWidget::copyOutput()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->encodedEdit->toPlainText());
}
