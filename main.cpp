#include "devtoolsmainwindow.h"

#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <type_traits>

#include <QApplication>
#include <QLocale>
#include <QSharedPointer>
#include <QTranslator>

#include "build.hpp"
#include "config/engine/configjsonfileengine.hpp"
#include "log/loggable.hpp"
#include "log/qtfmt.hpp"
#include "utility/appinformation.hpp"
#include "utility/pathregistry.hpp"

constexpr std::size_t MAX_LOG_SIZE = 1024 * 1024 * 2;  // 2MB
constexpr std::size_t MAX_LOG_COUNT = 50;

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    QApplication::setApplicationName(APP_NAME);
    QApplication::setApplicationDisplayName(APP_NAME);
    QApplication::setApplicationVersion(QString("%1.%2.%3")
                                            .arg(VERSION_MAJOR)
                                            .arg(VERSION_MINOR)
                                            .arg(VERSION_PATCH));

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString& locale : uiLanguages) {
        const QString baseName = "devtools_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);

            break;
        }
    }

    auto registry =
        QSharedPointer<std::conditional<IS_PORTABLE, UserOnlyPathRegistry,
                                        SystemPathRegistry>::type>::create();
    registry->cache();

    auto configEngine = QSharedPointer<ConfigJsonFileEngine>::create(
        registry->get<PathRegistry::Config>() + "/config.json",
        ConfigEngine::FlushPolicy{
            ConfigEngine::FlushPolicy::Type::QueuedAfterChange});

    auto sinks = std::vector<std::shared_ptr<spdlog::sinks::sink>>{
        std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            registry->native<PathRegistry::Log>() +
                QString("/%1.log")
                    .arg(QDate::currentDate().toString("yyyy-MM-dd"))
                    .toStdString(),
            MAX_LOG_SIZE, MAX_LOG_COUNT, true),
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};

    auto logger = std::make_shared<spdlog::logger>(APP_NAME, std::begin(sinks),
                                                   std::end(sinks));
    spdlog::set_default_logger(logger);
    register_default_logger(logger);
    spdlog::set_pattern(
        "[%Y-%m-%d %X.%e%z] [process %P] [thread %t] [%n] [%l] %v");
    logger->info("Registered path, Type: Data, Path: {}",
                 registry->get<PathRegistry::Data>());
    logger->info("Registered path, Type: Config, Path: {}",
                 registry->get<PathRegistry::Config>());
    logger->info("Registered path, Type: Log, Path: {}",
                 registry->get<PathRegistry::Log>());

    auto appinformation = QSharedPointer<AppInformation>::create();
    appinformation->set(AppInformation::Type::Name, APP_NAME);
    appinformation->set(AppInformation::Type::Author, APP_AUTHOR);
    appinformation->set(AppInformation::Type::SupportMail, APP_AUTHOR_MAIL);
    appinformation->set(AppInformation::Type::SupportWebpage,
                        APP_AUTHOR_WEBPAGE);
    appinformation->set(AppInformation::Type::SupportGithub, APP_AUTHOR_GITHUB);
    appinformation->set(AppInformation::Type::Type,
                        IS_PORTABLE ? "Portable" : "Install");
    appinformation->set(AppInformation::Type::LogoPath, ":/licenses/spdlog");
    appinformation->set(AppInformation::Type::Version, QString("%1.%2.%3")
                                                           .arg(VERSION_MAJOR)
                                                           .arg(VERSION_MINOR)
                                                           .arg(VERSION_PATCH));
    appinformation->set(AppInformation::Type::Version_Long, VERSION_STRING);

    qInstallMessageHandler(message_handler);

    DevToolsMainWindow::Dependency dependency(configEngine, registry,
                                              appinformation);

    DevToolsMainWindow w(dependency);
    w.show();
    return a.exec();
}
