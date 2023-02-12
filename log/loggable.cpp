#include "loggable.hpp"

#include "spdlog/spdlog.h"
#include <QString>
#include <mutex>

namespace internal {

std::mutex get_or_make_mutex;

std::shared_ptr<spdlog::logger> get_or_make_logger(const char* name) {
    std::lock_guard<std::mutex> guard(get_or_make_mutex);
    auto logger = spdlog::get(name);
    if (!logger) {
        auto defaultLogger = spdlog::default_logger();
        logger = std::make_shared<spdlog::logger>(
            name, std::begin(defaultLogger->sinks()),
            std::end(defaultLogger->sinks()));
        spdlog::register_logger(logger);
    }
    return logger;
}

}  // namespace internal

Loggable::Loggable(const char* name) {
    logger = internal::get_or_make_logger(name);
}

void message_handler(QtMsgType type, const QMessageLogContext& context,
                     const QString& message) {
    Q_UNUSED(context)
    auto logger = spdlog::default_logger();
    if (!logger)
        return;
    switch (type) {
        case QtDebugMsg:
            logger->debug(message.toStdString());
            break;
        case QtInfoMsg:
            logger->info(message.toStdString());
            break;
        case QtWarningMsg:
            logger->warn(message.toStdString());
            break;
        case QtCriticalMsg:
            logger->error(message.toStdString());
            break;
        case QtFatalMsg:
            logger->critical(message.toStdString());
            break;
    }
}

void register_default_logger(std::shared_ptr<spdlog::logger> defaultLogger) {
    spdlog::set_default_logger(defaultLogger);
}
