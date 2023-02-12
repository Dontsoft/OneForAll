#include "devtoolsmainwindow.h"
#include "./ui_devtoolsmainwindow.h"

enum class Depenencies {
    ConfigEngine = 0,
    PathRegistry = 1,
    AppInformation = 2
};

DevToolsMainWindow::DevToolsMainWindow(const Dependency& dependency,
                                       QWidget* parent)
    : QMainWindow(parent),
      Dependant<QSharedPointer<ConfigEngine>, QSharedPointer<PathRegistry>,
                QSharedPointer<AppInformation>>(dependency),
      Loggable("DevTools"),
      ui(new Ui::DevToolsMainWindow) {
    ui->setupUi(this);
}

DevToolsMainWindow::~DevToolsMainWindow() { delete ui; }
