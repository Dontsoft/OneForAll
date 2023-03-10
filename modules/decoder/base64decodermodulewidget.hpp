#ifndef BASE64DECODERMODULEWIDGET_HPP
#define BASE64DECODERMODULEWIDGET_HPP

#include <QWidget>

namespace Ui {
    class Base64DecoderModuleWidget;
}

class Base64DecoderModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Base64DecoderModuleWidget(QWidget *parent = nullptr);
    ~Base64DecoderModuleWidget();

public slots:
    void decoded(const QByteArray& byteArray);

signals:
    void decode(const QString& encodedString);

private slots:

    void clearInput();
    void clearOutput();
    void copyOutput();

    void filenameChanged(const QString& fileName);
    void decodeButtonClicked();
    void saveAsFile();

private:
    Ui::Base64DecoderModuleWidget* ui;
    QByteArray _output;
};

#endif // BASE64DECODERMODULEWIDGET_HPP
