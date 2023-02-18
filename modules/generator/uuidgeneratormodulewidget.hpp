#ifndef UUIDGENERATORMODULEWIDGET_HPP
#define UUIDGENERATORMODULEWIDGET_HPP

#include <QWidget>

namespace Ui {
    class UUIDGeneratorModuleWidget;
}

class UUIDGeneratorModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UUIDGeneratorModuleWidget(QWidget *parent = nullptr);
    ~UUIDGeneratorModuleWidget();

private:
    Ui::UUIDGeneratorModuleWidget *ui;
};

#endif // UUIDGENERATORMODULEWIDGET_HPP
