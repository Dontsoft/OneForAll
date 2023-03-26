#include <QApplication>
#include <QLocale>
#include <QSharedPointer>
#include <QTranslator>
#include <type_traits>

#include "log/loggable.hpp"
#include "log/qtfmt.hpp"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#include "build.hpp"
#include "config/engine/configjsonfileengine.hpp"
#include "devtoolsmainwindow.h"
#include "global.hpp"
#include "modules/decoder/base64decodermodule.hpp"
#include "modules/decoder/uuiddecodermodule.hpp"
#include "modules/encoder/base64encodermodule.hpp"
#include "modules/generator/passwordgeneratormodule.hpp"
#include "modules/generator/uuidgeneratormodule.hpp"
#include "random/random.hpp"
#include "utility/appinformation.hpp"
#include "utility/pathregistry.hpp"

constexpr std::size_t MAX_LOG_SIZE = 1024 * 1024 * 2; // 2MB
constexpr std::size_t MAX_LOG_COUNT = 50;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int returnCode = 0;
    do
    {
        QApplication::setApplicationName(APP_NAME);
        QApplication::setApplicationDisplayName(APP_NAME);
        QApplication::setApplicationVersion(QString("%1.%2.%3")
                                                .arg(VERSION_MAJOR)
                                                .arg(VERSION_MINOR)
                                                .arg(VERSION_PATCH));

        QTranslator translator;
        const QStringList uiLanguages = QLocale::system().uiLanguages();

        for (const QString &locale : uiLanguages)
        {
            const QString baseName = "devtools_" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName))
            {
                a.installTranslator(&translator);
                break;
            }
        }

        auto registry = QSharedPointer<
            std::conditional<IS_PORTABLE, UserOnlyPathRegistry,
                             SystemPathRegistry>::type>::create();
        registry->cache();

        auto configEngine = QSharedPointer<ConfigJsonFileEngine>::create(
            registry->get<PathRegistry::Config>() + "/config.json",
            ConfigEngine::FlushPolicy{
                ConfigEngine::FlushPolicy::Type::QueuedAfterChange});

        auto sinks = std::vector<std::shared_ptr<spdlog::sinks::sink>>{
            std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                registry->native<PathRegistry::Log>()
                    + QString("/%1.log")
                          .arg(QDate::currentDate().toString("yyyy-MM-dd"))
                          .toStdString(),
                MAX_LOG_SIZE, MAX_LOG_COUNT, true),
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>()};

        auto logger = std::make_shared<spdlog::logger>(
            APP_NAME, std::begin(sinks), std::end(sinks));
        spdlog::set_default_logger(logger);
        register_default_logger(logger);
        qInstallMessageHandler(message_handler);
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
        appinformation->set(AppInformation::Type::SupportGithub,
                            APP_AUTHOR_GITHUB);
        appinformation->set(AppInformation::Type::Type,
                            IS_PORTABLE ? "Portable" : "Install");
        appinformation->set(AppInformation::Type::LogoPath,
                            "://OneForAll_128x128.png");
        appinformation->set(AppInformation::Type::Version,
                            QString("%1.%2.%3")
                                .arg(VERSION_MAJOR)
                                .arg(VERSION_MINOR)
                                .arg(VERSION_PATCH));
        appinformation->set(AppInformation::Type::Version_Long, VERSION_STRING);
        appinformation->set(AppInformation::Type::License, "://LICENSE");

        appinformation->addThirdPartyInformation(
            {.type = AppInformation::ThirdPartyInformation::Type::Library,
             .values = {{AppInformation::Type::Name, QString("spdlog")},
                        {AppInformation::Type::Author, QString("Gabi Melman")},
                        {AppInformation::Type::SupportGithub,
                         QString("https://github.com/gabime/spdlog")},
                        {AppInformation::Type::Version, QString("1.11.0")},
                        {AppInformation::Type::License,
                         QString(":/third-party/spdlog/LICENSE")}}});

        appinformation->addThirdPartyInformation(
            {.type = AppInformation::ThirdPartyInformation::Type::Library,
             .values
             = {{AppInformation::Type::Name, QString("nlohmann-json")},
                {AppInformation::Type::Author, QString("Niels Lohmann")},
                {AppInformation::Type::SupportGithub,
                 QString("https://github.com/nlohmann/json")},
                {AppInformation::Type::Version, QString("3.11.2")},
                {AppInformation::Type::License,
                 QString(":/third-party/nlohmann-json/LICENSE")}}});

        appinformation->addThirdPartyInformation(
            {.type = AppInformation::ThirdPartyInformation::Type::Library,
             .values = {{AppInformation::Type::Name, QString("yaml-cpp")},
                        {AppInformation::Type::Author, QString("Jesse Beder")},
                        {AppInformation::Type::SupportGithub,
                         QString("https://github.com/jbeder/yaml-cpp")},
                        {AppInformation::Type::Version, QString("0.7.0")},
                        {AppInformation::Type::License,
                         QString(":/third-party/yaml-cpp/LICENSE")}}});

        appinformation->addThirdPartyInformation(
            {.type = AppInformation::ThirdPartyInformation::Type::Library,
             .values
             = {{AppInformation::Type::Name, QString("stduuid")},
                {AppInformation::Type::Author, QString("Marius Bancila")},
                {AppInformation::Type::SupportGithub,
                 QString("https://github.com/mariusbancila/stduuid")},
                {AppInformation::Type::Version, QString("1.2.3")},
                {AppInformation::Type::License,
                 QString(":/third-party/stduuid/LICENSE")}}});

        appinformation->addThirdPartyInformation(
            {.type = AppInformation::ThirdPartyInformation::Type::Library,
             .values = {{AppInformation::Type::Name, QString("Apache Arrow")},
                        {AppInformation::Type::Author,
                         QString("The Apache Software Foundation")},
                        {AppInformation::Type::SupportWebpage,
                         QString("https://arrow.apache.org")},
                        {AppInformation::Type::Version, QString("11.0.0")},
                        {AppInformation::Type::License,
                         QString(":/third-party/apache-arrow/LICENSE")}}});

        appinformation->addThirdPartyInformation(
            {.type = AppInformation::ThirdPartyInformation::Type::Library,
             .values = {{AppInformation::Type::Name, QString("tinyxml2")},
                        {AppInformation::Type::Author, QString("Lee Thomason")},
                        {AppInformation::Type::SupportGithub,
                         QString("https://github.com/leethomason/tinyxml2")},
                        {AppInformation::Type::Version, QString("9.0.0")},
                        {AppInformation::Type::License,
                         QString(":/third-party/tinyxml2/LICENSE")}}});

        appinformation->addThirdPartyInformation(
            {.type = AppInformation::ThirdPartyInformation::Type::Library,
             .values
             = {{AppInformation::Type::Name, QString("simdjson")},
                {AppInformation::Type::Author, QString("Daniel Lemire")},
                {AppInformation::Type::SupportGithub,
                 QString("https://github.com/simdjson/simdjson")},
                {AppInformation::Type::Version, QString("3.1.6")},
                {AppInformation::Type::License,
                 QString(":/third-party/simdjson/LICENSE")}}});

        appinformation->addThirdPartyInformation(
            {.type = AppInformation::ThirdPartyInformation::Type::Library,
             .values = {{AppInformation::Type::Name, QString("spy")},
                        {AppInformation::Type::Author, QString("Joel Falcou")},
                        {AppInformation::Type::SupportGithub,
                         QString("https://github.com/jfalcou/spy")},
                        {AppInformation::Type::Version, QString("1.0.0")},
                        {AppInformation::Type::License,
                         QString(":/third-party/spy/LICENSE")}}});

        auto moduleRegistry = QSharedPointer<ModuleRegistry>::create();
        moduleRegistry->addModule(QSharedPointer<UUIDDecoderModule>::create());
        moduleRegistry->addModule(QSharedPointer<UUIDGeneratorModule>::create(
            UUIDGeneratorModule::Dependency{configEngine}));
        moduleRegistry->addModule(
            QSharedPointer<PasswordGeneratorModule>::create(
                PasswordGeneratorModule::Dependency{
                    QSharedPointer<Random>::create()}));
        moduleRegistry->addModule(
            QSharedPointer<Base64EncoderModule>::create());
        moduleRegistry->addModule(
            QSharedPointer<Base64DecoderModule>::create());
        DevToolsMainWindow::Dependency dependency(
            configEngine, registry, appinformation, moduleRegistry);

        DevToolsMainWindow w(dependency);
        w.show();
        returnCode = a.exec();
        w.close();
    } while (returnCode == RESTART_RETURN_CODE);
    return returnCode;
}
