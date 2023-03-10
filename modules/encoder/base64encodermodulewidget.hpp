#ifndef BASE64ENCODERMODULEWIDGET_HPP
#define BASE64ENCODERMODULEWIDGET_HPP

#include <QWidget>

namespace Ui {
    class Base64EncoderModuleWidget;
}

class Base64EncoderModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Base64EncoderModuleWidget(QWidget *parent = nullptr);
    ~Base64EncoderModuleWidget();

signals:
    void encode(const QByteArray &byteArray);
    void base64OptionsChanged(QByteArray::Base64Options options);
public slots:
    void encoded(const QString &encodedContent);

private slots:

    void copyOutput();

    void base64OptionsComboBoxChanged(int index);

    void selectFile();
    void fileChanged(const QString &fileName);
    void encodeFileContent();

    void textChanged();

    void saveEncodedAsFiled();

private:
    Ui::Base64EncoderModuleWidget *ui;
};

#endif // BASE64ENCODERMODULEWIDGET_HPP
