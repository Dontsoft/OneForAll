#ifndef DEVTOOLSMAINWINDOW_H
#define DEVTOOLSMAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class DevToolsMainWindow; }
QT_END_NAMESPACE

class DevToolsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    DevToolsMainWindow(QWidget *parent = nullptr);
    ~DevToolsMainWindow();

private:
    Ui::DevToolsMainWindow *ui;
};
#endif // DEVTOOLSMAINWINDOW_H
