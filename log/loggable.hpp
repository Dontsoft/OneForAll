#ifndef LOGGABLE_HPP
#define LOGGABLE_HPP

#include <QString>

#include <memory>
#include <string>

#include <spdlog/fmt/ostr.h>

namespace spdlog {
class logger;
}

class Loggable {
   protected:
    Loggable(const char* name);
    std::shared_ptr<spdlog::logger> logger;
};

void message_handler(QtMsgType type, const QMessageLogContext& context,
                     const QString& message);
void register_default_logger(std::shared_ptr<spdlog::logger> defaultLogger);

template <>
struct fmt::formatter<QString> : fmt::formatter<std::string> {
    auto format(const QString& c, format_context& ctx) -> decltype(ctx.out()) {
        return format_to(ctx.out(), "{}", c.toStdString());
    }
};
#endif  // LOGGABLE_HPP
