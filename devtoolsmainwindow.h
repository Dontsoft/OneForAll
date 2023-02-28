#ifndef DEVTOOLSMAINWINDOW_H
#define DEVTOOLSMAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "config/configengine.hpp"
#include "core/dependant.hpp"
#include "log/loggable.hpp"
#include "modules/moduleregistry.hpp"
#include "utility/appinformation.hpp"
#include "utility/pathregistry.hpp"

class ModuleNavigationModel;
class ModuleNavigationFilterModel;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class DevToolsMainWindow;
}
QT_END_NAMESPACE

class DevToolsMainWindow
    : public QMainWindow,
      public Dependant<
          QSharedPointer<ConfigEngine>, QSharedPointer<PathRegistry>,
          QSharedPointer<AppInformation>, QSharedPointer<ModuleRegistry>>,
      public Loggable
{
    Q_OBJECT

public:
    DevToolsMainWindow(const DevToolsMainWindow::Dependency& dependency,
                       QWidget* parent = nullptr);
    ~DevToolsMainWindow();
public slots:
    void updateWindowTitle(const QString& title = QString());
    void searchStringChanged(const QString& text);

private slots:
    void handleTreeViewSelection(const QModelIndex& current,
                                 const QModelIndex& previous);

private:
    Ui::DevToolsMainWindow* ui;
    ModuleNavigationFilterModel* _navigationModel;
};
#endif // DEVTOOLSMAINWINDOW_H
