#include "configyamlfileengine.hpp"

#include <QDebug>

#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <QDate>
#include <QDateTime>
#include <QDir>
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

#include "yaml-cpp/yaml.h"

namespace YAML {
template <>
struct convert<QString> {
    static Node encode(const QString& value) {
        return Node(value.toStdString());
    }

    static bool decode(const Node& node, QString& value) {
        if (!node.IsScalar())
            return false;
        value = QString::fromStdString(node.as<std::string>());
        return true;
    }
};

template <>
struct convert<QVariant> {
    static Node encode(const QVariant& value) {
        switch (static_cast<QMetaType::Type>(value.metaType().id())) {
            case QMetaType::Type::Bool:
                return Node(value.toBool());
            case QMetaType::Type::Int:
            case QMetaType::Type::Long:
            case QMetaType::Type::LongLong:
            case QMetaType::Type::Short:
            case QMetaType::Type::Char:
            case QMetaType::Type::Char16:
            case QMetaType::Type::Char32:
            case QMetaType::Type::SChar:
                return Node(value.toLongLong());
            case QMetaType::Type::UInt:
            case QMetaType::Type::ULong:
            case QMetaType::Type::ULongLong:
            case QMetaType::Type::UShort:
            case QMetaType::Type::UChar:
                return Node(value.toULongLong());
            case QMetaType::Type::Double:
            case QMetaType::Type::Float:
                return Node(value.toDouble());
            case QMetaType::Type::QString:
                return Node(value.toString());
            case QMetaType::Type::QLine: {
                const auto line = value.toLine();
                Node node;
                node["_type"] = "@Line";
                node["_data"]["x1"] = line.x1();
                node["_data"]["y1"] = line.y1();
                node["_data"]["x2"] = line.x2();
                node["_data"]["y2"] = line.y2();
                break;
            }
            case QMetaType::Type::QLineF: {
                const auto line = value.toLineF();
                Node node;
                node["_type"] = "@LineF";
                node["_data"]["x1"] = line.x1();
                node["_data"]["y1"] = line.y1();
                node["_data"]["x2"] = line.x2();
                node["_data"]["y2"] = line.y2();
                break;
            }
            case QMetaType::Type::QSize: {
                const auto size = value.toSize();
                Node node;
                node["_type"] = "@Size";
                node["_data"]["width"] = size.width();
                node["_data"]["height"] = size.height();
                return node;
            }
            case QMetaType::Type::QSizeF: {
                const auto size = value.toSizeF();
                Node node;
                node["_type"] = "@SizeF";
                node["_data"]["width"] = size.width();
                node["_data"]["height"] = size.height();
                return node;
            }
            case QMetaType::Type::QPoint: {
                const auto point = value.toPoint();
                Node node;
                node["_type"] = "@Point";
                node["_data"]["x"] = point.x();
                node["_data"]["y"] = point.y();
                return node;
            }
            case QMetaType::Type::QPointF: {
                const auto point = value.toPointF();
                Node node;
                node["_type"] = "@PointF";
                node["_data"]["x"] = point.x();
                node["_data"]["y"] = point.y();
                return node;
            }
            case QMetaType::Type::QRect: {
                const auto rect = value.toRect();
                Node node;
                node["_type"] = "@Rect";
                node["_data"]["x"] = rect.x();
                node["_data"]["y"] = rect.y();
                node["_data"]["width"] = rect.width();
                node["_data"]["height"] = rect.height();
                return node;
            }
            case QMetaType::Type::QRectF: {
                const auto rect = value.toRectF();
                Node node;
                node["_type"] = "@RectF";
                node["_data"]["x"] = rect.x();
                node["_data"]["y"] = rect.y();
                node["_data"]["width"] = rect.width();
                node["_data"]["height"] = rect.height();
                break;
            }
            case QMetaType::Type::QDate: {
                Node node;
                node["_type"] = "@Date";
                node["_data"] = value.toDate().toString("dd.MM.yyyy");
                return node;
            }
            case QMetaType::Type::QDateTime: {
                Node node;
                node["_type"] = "@DateTime";
                node["_data"] =
                    value.toDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz");
                return node;
            }
            case QMetaType::Type::QTime: {
                Node node;
                node["_type"] = "@Time";
                node["_data"] = value.toTime().toString("hh:mm:ss.zzz");
                return node;
            }
            case QMetaType::Type::QUuid: {
                Node node;
                node["_type"] = "@Uuid";
                node["_data"] = value.toUuid().toString();
                return node;
            }
            case QMetaType::Type::QStringList: {
                Node node;
                node["_type"] = "@StringList";
                const auto stringList = value.toStringList();
                std::vector<std::string> stdStringList;
                std::transform(
                    std::begin(stringList), std::end(stringList),
                    std::back_inserter(stdStringList),
                    [](const QString& _entry) { return _entry.toStdString(); });
                node["_data"] = stdStringList;
                return node;
            }
            case QMetaType::Type::QRegularExpression: {
                Node node;
                node["_type"] = "@RegularExpression";
                node["_data"] = value.toRegularExpression().pattern();
                return node;
            }
            case QMetaType::Type::QUrl: {
                Node node;
                node["_type"] = "@Url";
                node["_data"] = value.toUrl().toString();
                return node;
            }
            case QMetaType::Type::QByteArray: {
                Node node;
                node["_type"] = "@ByteArray";
                node["_data"] =
                    QString::fromUtf8(value.toByteArray().toBase64());
                return node;
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
                Node node;
                node["_type"] = "@Variant";
                node["_data"] = valueString;
                return node;
            }
        }
        return Node();
    }

    static bool decode(const Node& node, QVariant& value) {
        if (node.IsScalar()) {
            bool booleanValue;
            qlonglong qlonglongValue;
            qulonglong qulonglongValue;
            double doubleValue;
            if (YAML::convert<bool>::decode(node, booleanValue)) {
                value = QVariant::fromValue(booleanValue);
                return true;
            } else if (YAML::convert<qlonglong>::decode(node, qlonglongValue)) {
                value = QVariant::fromValue(qlonglongValue);
                return true;
            } else if (YAML::convert<qulonglong>::decode(node,
                                                         qulonglongValue)) {
                value = QVariant::fromValue(qulonglongValue);
                return true;
            } else if (YAML::convert<double>::decode(node, doubleValue)) {
                value = QVariant::fromValue(doubleValue);
                return true;
            }
            value = QVariant::fromValue(node.as<QString>());
            return true;
        } else if (node.IsMap()) {
            try {
                const auto type = node["_type"].as<std::string>();
                if (type == "@Line") {
                    value = QVariant::fromValue(
                        QLine(node["_data"]["x1"].as<int>(),
                              node["_data"]["y1"].as<int>(),
                              node["_data"]["x2"].as<int>(),
                              node["_data"]["y2"].as<int>()));
                    return true;
                } else if (type == "@LineF") {
                    value = QVariant::fromValue(
                        QLineF(node["_data"]["x1"].as<double>(),
                               node["_data"]["y1"].as<double>(),
                               node["_data"]["x2"].as<double>(),
                               node["_data"]["y2"].as<double>()));
                    return true;
                } else if (type == "@Size") {
                    value = QVariant::fromValue(
                        QSize(node["_data"]["width"].as<int>(),
                              node["_data"]["height"].as<int>()));
                    return true;
                } else if (type == "@SizeF") {
                    value = QVariant::fromValue(
                        QSize(node["_data"]["width"].as<double>(),
                              node["_data"]["height"].as<double>()));
                    return true;
                } else if (type == "@Point") {
                    value = QVariant::fromValue(
                        QPoint(node["_data"]["x"].as<int>(),
                               node["_data"]["y"].as<int>()));
                    return true;
                } else if (type == "@PointF") {
                    value = QVariant::fromValue(
                        QPointF(node["_data"]["x"].as<double>(),
                                node["_data"]["y"].as<double>()));
                    return true;
                } else if (type == "@Rect") {
                    value = QVariant::fromValue(
                        QRect(node["_data"]["x"].as<int>(),
                              node["_data"]["y"].as<int>(),
                              node["_data"]["width"].as<int>(),
                              node["_data"]["height"].as<int>()));
                    return true;
                } else if (type == "@RectF") {
                    value = QVariant::fromValue(
                        QRectF(node["_data"]["x"].as<double>(),
                               node["_data"]["y"].as<double>(),
                               node["_data"]["width"].as<double>(),
                               node["_data"]["height"].as<double>()));
                    return true;
                } else if (type == "@Date") {
                    value = QVariant::fromValue(QDate::fromString(
                        node["_data"].as<QString>(), "dd.MM.yyyy"));
                    return true;
                } else if (type == "@DateTime") {
                    value = QVariant::fromValue(
                        QDateTime::fromString(node["_data"].as<QString>(),
                                              "dd.MM.yyyy hh:mm:ss.zzz"));
                    return true;
                } else if (type == "@Time") {
                    value = QVariant::fromValue(QTime::fromString(
                        node["_data"].as<QString>(), "hh:mm:ss.zzz"));
                    return true;
                } else if (type == "@Uuid") {
                    value = QVariant::fromValue(
                        QUuid::fromString(node["_data"].as<QString>()));
                    return true;
                } else if (type == "@StringList") {
                    auto stdStringList =
                        node["_data"].as<std::vector<std::string>>();
                    QStringList stringList;
                    std::transform(std::begin(stdStringList),
                                   std::end(stdStringList),
                                   std::back_inserter(stringList),
                                   [](const std::string& str) {
                                       return QString::fromStdString(str);
                                   });
                    value = QVariant::fromValue(stringList);
                    return true;
                } else if (type == "@RegularExpression") {
                    value = QVariant::fromValue(
                        QRegularExpression(node["_data"].as<QString>()));
                    return true;
                } else if (type == "@Url") {
                    value =
                        QVariant::fromValue(QUrl(node["_data"].as<QString>()));
                    return true;
                } else if (type == "@ByteArray") {
                    value = QVariant::fromValue(QByteArray::fromBase64(
                        node["_data"].as<QString>().toUtf8()));
                    return true;
                } else if (type == "@Variant") {
                    auto data = QByteArray::fromBase64(
                        node["_data"].as<QString>().toUtf8());
                    QBuffer buffer(&data);
                    if (buffer.open(QIODevice::ReadOnly)) {
                        QDataStream readStream(&buffer);
                        readStream >> value;
                    }
                    return true;
                }
            } catch (...) {
            }
        }
        return false;
    }
};

}  // namespace YAML

ConfigYamlFileEngine::ConfigYamlFileEngine(
    const QString& filename, const ConfigEngine::FlushPolicy& flushPolicy)
    : ConfigFileEngine(filename, flushPolicy) {}

void flush_value(YAML::Node& node, QStringList nameStack,
                 const QVariant& value) {
    if (nameStack.isEmpty()) {
        node = value;
    } else {
        QString first = nameStack.takeFirst();
        node[first.toStdString()] = YAML::Node();
        flush_value(node, nameStack, value);
    }
}

void ConfigYamlFileEngine::flushImplementation(
    const QHash<QString, QVariant>& values) {
    auto node = YAML::Node();
    for (auto it = values.constBegin(); it != values.constEnd(); ++it) {
        QStringList list = it.key().split("/");
        flush_value(node, list, it.value());
    }
    QFile f(getFilename());
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&f);
        YAML::Emitter emitter;
        emitter << node;
        out << QString::fromStdString(emitter.c_str());
    }
}

void get_recursive(const YAML::Node& node, const QString& prefix,
                   QHash<QString, QVariant>& values) {
    if (node.IsMap()) {
        if (node["_type"].IsDefined() && node["_type"].IsDefined()) {
            QVariant value = node.as<QVariant>();
            values.insert(prefix, value);
            return;
        }
    } else {
        QVariant value = node.as<QVariant>();
        values.insert(prefix, value);
        return;
    }
    for (auto it = node.begin(); it != node.end(); ++it)
        get_recursive(it->second,
                      QString("%1%2%3").arg(
                          prefix, prefix.isEmpty() ? QString() : QString("/"),
                          QString::fromStdString(it->first.as<std::string>())),
                      values);
}

QHash<QString, QVariant> ConfigYamlFileEngine::getAll() const {
    QHash<QString, QVariant> values;
    try {
        auto node = YAML::LoadFile(
            QDir::toNativeSeparators(getFilename()).toStdString());
        get_recursive(node, QString(), values);
    } catch (...) {
    }
    return values;
}
