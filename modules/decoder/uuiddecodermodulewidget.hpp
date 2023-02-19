#ifndef UUIDDECODERMODULEWIDGET_HPP
#define UUIDDECODERMODULEWIDGET_HPP

#include <QWidget>

namespace Ui {
    class UUIDDecoderModuleWidget;
}

class UUIDDecoderModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UUIDDecoderModuleWidget(QWidget *parent = nullptr);
    ~UUIDDecoderModuleWidget();

public slots:
    void decode();

private:
    Ui::UUIDDecoderModuleWidget *ui;
};

#endif // UUIDDECODERMODULEWIDGET_HPP
