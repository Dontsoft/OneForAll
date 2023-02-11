#include "devtoolsmainwindow.h"
#include "./ui_devtoolsmainwindow.h"

DevToolsMainWindow::DevToolsMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DevToolsMainWindow)
{
    ui->setupUi(this);
}

DevToolsMainWindow::~DevToolsMainWindow()
{
    delete ui;
}
