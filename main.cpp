#include "devtoolsmainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();


    for (const QString &locale : uiLanguages) {
        const QString baseName = "devtools_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);

            break;
        }
    }


    DevToolsMainWindow w;
    w.show();
    return a.exec();
}
