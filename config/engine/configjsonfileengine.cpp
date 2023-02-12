#include "configjsonfileengine.hpp"

#include "nlohmann/json.hpp"

#include <QDebug>

#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <QDate>
#include <QDateTime>
#include <QFile>
#include <QLine>
#include <QLineF>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QRegularExpression>
#include <QSize>
#include <QSizeF>
#include <QUrl>
#include <QUuid>

namespace nlohmann {

template <>
struct adl_serializer<QString> {
    static void to_json(json& j, const QString& value) {
        j = value.toStdString();
    }

    static void from_json(const json& j, QString& value) {
        if (j.is_null())
            return;
        value = QString::fromStdString(j.get<std::string>());
    }
};

template <>
struct adl_serializer<QVariant> {
    static void to_json(json& j, const QVariant& value) {
        switch (static_cast<QMetaType::Type>(value.metaType().id())) {
            case QMetaType::Type::Bool:
                j = value.toBool();
                break;
            case QMetaType::Type::Int:
            case QMetaType::Type::Long:
            case QMetaType::Type::LongLong:
            case QMetaType::Type::Short:
            case QMetaType::Type::Char:
            case QMetaType::Type::Char16:
            case QMetaType::Type::Char32:
            case QMetaType::Type::SChar:
                j = value.toLongLong();
                break;
            case QMetaType::Type::UInt:
            case QMetaType::Type::ULong:
            case QMetaType::Type::ULongLong:
            case QMetaType::Type::UShort:
            case QMetaType::Type::UChar:
                j = value.toULongLong();
                break;
            case QMetaType::Type::Double:
            case QMetaType::Type::Float:
                j = value.toDouble();
                break;
            case QMetaType::Type::QString:
                j = value.toString();
                break;
            case QMetaType::Type::QLine: {
                const auto line = value.toLine();
                j = json::object();
                j["_type"] = "@Line";
                j["_data"] = json::object();
                j["_data"]["x1"] = line.x1();
                j["_data"]["y1"] = line.y1();
                j["_data"]["x2"] = line.x2();
                j["_data"]["y2"] = line.y2();
                break;
            }
            case QMetaType::Type::QLineF: {
                const auto line = value.toLineF();
                j = json::object();
                j["_type"] = "@LineF";
                j["_data"] = json::object();
                j["_data"]["x1"] = line.x1();
                j["_data"]["y1"] = line.y1();
                j["_data"]["x2"] = line.x2();
                j["_data"]["y2"] = line.y2();
                break;
            }
            case QMetaType::Type::QSize: {
                const auto size = value.toSize();
                j = json::object();
                j["_type"] = "@Size";
                j["_data"] = json::object();
                j["_data"]["width"] = size.width();
                j["_data"]["height"] = size.height();
                break;
            }
            case QMetaType::Type::QSizeF: {
                const auto size = value.toSizeF();
                j = json::object();
                j["_type"] = "@SizeF";
                j["_data"] = json::object();
                j["_data"]["width"] = size.width();
                j["_data"]["height"] = size.height();
                break;
            }
            case QMetaType::Type::QPoint: {
                const auto point = value.toPoint();
                j = json::object();
                j["_type"] = "@Point";
                j["_data"] = json::object();
                j["_data"]["x"] = point.x();
                j["_data"]["y"] = point.y();
                break;
            }
            case QMetaType::Type::QPointF: {
                const auto point = value.toPointF();
                j = json::object();
                j["_type"] = "@PointF";
                j["_data"] = json::object();
                j["_data"]["x"] = point.x();
                j["_data"]["y"] = point.y();
                break;
            }
            case QMetaType::Type::QRect: {
                const auto rect = value.toRect();
                j = json::object();
                j["_type"] = "@Rect";
                j["_data"] = json::object();
                j["_data"]["x"] = rect.x();
                j["_data"]["y"] = rect.y();
                j["_data"]["width"] = rect.width();
                j["_data"]["height"] = rect.height();
                break;
            }
            case QMetaType::Type::QRectF: {
                const auto rect = value.toRectF();
                j = json::object();
                j["_type"] = "@RectF";
                j["_data"] = json::object();
                j["_data"]["x"] = rect.x();
                j["_data"]["y"] = rect.y();
                j["_data"]["width"] = rect.width();
                j["_data"]["height"] = rect.height();
                break;
            }
            case QMetaType::Type::QDate: {
                j = json::object();
                j["_type"] = "@Date";
                j["_data"] = value.toDate().toString("dd.MM.yyyy");
                break;
            }
            case QMetaType::Type::QDateTime: {
                j = json::object();
                j["_type"] = "@DateTime";
                j["_data"] =
                    value.toDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
                break;
            }
            case QMetaType::Type::QTime: {
                j = json::object();
                j["_type"] = "@Time";
                j["_data"] = value.toTime().toString("hh:mm:ss.zzz");
                break;
            }
            case QMetaType::Type::QUuid: {
                j = json::object();
                j["_type"] = "@Uuid";
                j["_data"] = value.toUuid().toString();
                break;
            }
            case QMetaType::Type::QStringList: {
                j = json::object();
                j["_type"] = "@StringList";
                const auto stringList = value.toStringList();
                std::vector<std::string> stdStringList;
                std::transform(
                    std::begin(stringList), std::end(stringList),
                    std::back_inserter(stdStringList),
                    [](const QString& _entry) { return _entry.toStdString(); });
                j["_data"] = stdStringList;
                break;
            }
            case QMetaType::Type::QRegularExpression: {
                j = json::object();
                j["_type"] = "@RegularExpression";
                j["_data"] = value.toRegularExpression().pattern();
                break;
            }
            case QMetaType::Type::QUrl: {
                j = json::object();
                j["_type"] = "@Url";
                j["_data"] = value.toUrl().toString();
                break;
            }
            case QMetaType::Type::QByteArray: {
                j = json::object();
                j["_type"] = "@ByteArray";
                j["_data"] = QString::fromUtf8(value.toByteArray().toBase64());
                break;
            }
            default: {
                QByteArray valueData;
                {
                    QBuffer buffer(&valueData);
                    if (buffer.open(QIODevice::WriteOnly)) {
                        QDataStream bufferStream(&buffer);
                        bufferStream << value;
                    }
                }
                QString valueString = QString::fromUtf8(valueData.toBase64());
                j = json::object();
                j["_type"] = "@Variant";
                j["_data"] = valueString;
                break;
            }
        }
    }

    static void from_json(const json& j, QVariant& value) {
        if (j.is_boolean()) {
            value = QVariant::fromValue(j.get<bool>());
            return;
        } else if (j.is_number_float()) {
            value = QVariant::fromValue(j.get<double>());
            return;
        } else if (j.is_number_unsigned()) {
            value = QVariant::fromValue(j.get<qulonglong>());
            return;
        } else if (j.is_number_integer()) {
            value = QVariant::fromValue(j.get<qlonglong>());
            return;
        } else if (j.is_string()) {
            value = QVariant::fromValue(j.get<QString>());
            return;
        } else if (j.is_object()) {
            const auto type = j["_type"].get<std::string>();
            if (type == "@Line") {
                value = QVariant::fromValue(QLine(
                    j["_data"]["x1"].get<int>(), j["_data"]["y1"].get<int>(),
                    j["_data"]["x2"].get<int>(), j["_data"]["y2"].get<int>()));
                return;
            } else if (type == "@LineF") {
                value =
                    QVariant::fromValue(QLineF(j["_data"]["x1"].get<double>(),
                                               j["_data"]["y1"].get<double>(),
                                               j["_data"]["x2"].get<double>(),
                                               j["_data"]["y2"].get<double>()));
                return;
            } else if (type == "@Size") {
                value =
                    QVariant::fromValue(QSize(j["_data"]["width"].get<int>(),
                                              j["_data"]["height"].get<int>()));
                return;
            } else if (type == "@SizeF") {
                value = QVariant::fromValue(
                    QSize(j["_data"]["width"].get<double>(),
                          j["_data"]["height"].get<double>()));
                return;
            } else if (type == "@Point") {
                value = QVariant::fromValue(QPoint(j["_data"]["x"].get<int>(),
                                                   j["_data"]["y"].get<int>()));
                return;
            } else if (type == "@PointF") {
                value =
                    QVariant::fromValue(QPointF(j["_data"]["x"].get<double>(),
                                                j["_data"]["y"].get<double>()));
                return;
            } else if (type == "@Rect") {
                value = QVariant::fromValue(QRect(
                    j["_data"]["x"].get<int>(), j["_data"]["y"].get<int>(),
                    j["_data"]["width"].get<int>(),
                    j["_data"]["height"].get<int>()));
                return;
            } else if (type == "@RectF") {
                value = QVariant::fromValue(
                    QRectF(j["_data"]["x"].get<double>(),
                           j["_data"]["y"].get<double>(),
                           j["_data"]["width"].get<double>(),
                           j["_data"]["height"].get<double>()));
                return;
            } else if (type == "@Date") {
                value = QVariant::fromValue(
                    QDate::fromString(j["_data"].get<QString>(), "dd.MM.yyyy"));
                return;
            } else if (type == "@DateTime") {
                value = QVariant::fromValue(QDateTime::fromString(
                    j["_data"].get<QString>(), "dd.MM.yyyy hh:mm:ss.zzz"));
                return;
            } else if (type == "@Time") {
                value = QVariant::fromValue(QTime::fromString(
                    j["_data"].get<QString>(), "hh:mm:ss.zzz"));
                return;
            } else if (type == "@Uuid") {
                value = QVariant::fromValue(
                    QUuid::fromString(j["_data"].get<QString>()));
                return;
            } else if (type == "@StringList") {
                auto stdStringList = j["_data"].get<std::vector<std::string>>();
                QStringList stringList;
                std::transform(
                    std::begin(stdStringList), std::end(stdStringList),
                    std::back_inserter(stringList), [](const std::string& str) {
                        return QString::fromStdString(str);
                    });
                value = QVariant::fromValue(stringList);
                return;
            } else if (type == "@RegularExpression") {
                value = QVariant::fromValue(
                    QRegularExpression(j["_data"].get<QString>()));
                return;
            } else if (type == "@Url") {
                value = QVariant::fromValue(QUrl(j["_data"].get<QString>()));
                return;
            } else if (type == "@ByteArray") {
                value = QVariant::fromValue(
                    QByteArray::fromBase64(j["_data"].get<QString>().toUtf8()));
                return;
            } else if (type == "@Variant") {
                auto data =
                    QByteArray::fromBase64(j["_data"].get<QString>().toUtf8());
                QBuffer buffer(&data);
                if (buffer.open(QIODevice::ReadOnly)) {
                    QDataStream readStream(&buffer);
                    readStream >> value;
                }
                return;
            }
        }
    }
};

}  // namespace nlohmann

ConfigJsonFileEngine::ConfigJsonFileEngine(
    const QString& filename, const ConfigEngine::FlushPolicy& flushPolicy)
    : ConfigFileEngine(filename, flushPolicy) {}

void flush_value(nlohmann::json& j, QStringList nameStack,
                 const QVariant& value) {
    if (nameStack.isEmpty()) {
        j = value;
    } else {
        QString first = nameStack.takeFirst();
        flush_value(j[first.toStdString()], nameStack, value);
    }
}

void ConfigJsonFileEngine::flushImplementation(
    const QHash<QString, QVariant>& values) {
    auto j = nlohmann::json::object();
    for (auto it = values.constBegin(); it != values.constEnd(); ++it) {
        QStringList list = it.key().split("/");
        flush_value(j, list, it.value());
    }
    QFile f(getFilename());
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&f);
        out << QString::fromStdString(j.dump(2));
    }
}

void get_recursive(const nlohmann::json& j, const QString& prefix,
                   QHash<QString, QVariant>& values) {
    if (j.is_object()) {
        if (j.find("_type") != j.end() && j.find("_data") != j.end()) {
            QVariant value = j.get<QVariant>();
            values.insert(prefix, value);
            return;
        }
    } else {
        QVariant value = j.get<QVariant>();
        values.insert(prefix, value);
        return;
    }
    for (auto it = j.begin(); it != j.end(); ++it)
        get_recursive(it.value(),
                      QString("%1%2%3").arg(
                          prefix, prefix.isEmpty() ? QString() : QString("/"),
                          QString::fromStdString(it.key())),
                      values);
}

QHash<QString, QVariant> ConfigJsonFileEngine::getAll() const {
    QHash<QString, QVariant> values;
    try {
        QFile f(getFilename());
        if (f.open(QIODevice::ReadOnly)) {
            auto content = f.readAll().toStdString();
            auto j = nlohmann::json::parse(content);
            get_recursive(j, QString(), values);
        }
    } catch (...) {
    }
    return values;
}
