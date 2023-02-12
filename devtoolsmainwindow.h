#ifndef DEVTOOLSMAINWINDOW_H
#define DEVTOOLSMAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "config/configengine.hpp"
#include "core/dependant.hpp"
#include "log/loggable.hpp"
#include "utility/appinformation.hpp"
#include "utility/pathregistry.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class DevToolsMainWindow;
}
QT_END_NAMESPACE

class DevToolsMainWindow : public QMainWindow,
                           public Dependant<QSharedPointer<ConfigEngine>,
                                            QSharedPointer<PathRegistry>,
                                            QSharedPointer<AppInformation>>,
                           public Loggable {
    Q_OBJECT

   public:
    DevToolsMainWindow(const DevToolsMainWindow::Dependency& dependency,
                       QWidget* parent = nullptr);
    ~DevToolsMainWindow();

   private:
    Ui::DevToolsMainWindow* ui;
};
#endif  // DEVTOOLSMAINWINDOW_H
