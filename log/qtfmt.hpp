#ifndef QT_FMT_HPP
#define QT_FMT_HPP

#include "spdlog/fmt/ostr.h"

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
#include <QVariant>

#include <ostream>

#include "loggable.hpp"

QString to_string(const QLine& line) {
    return QString("(x1 = %1, y1 = %2, x2 = %3, y2 = %4)")
        .arg(line.x1())
        .arg(line.y1())
        .arg(line.x2())
        .arg(line.y2());
}

QString to_string(const QLineF& line) {
    return QString("(x1 = %1, y1 = %2, x2 = %3, y2 = %4)")
        .arg(line.x1())
        .arg(line.y1())
        .arg(line.x2())
        .arg(line.y2());
}

QString to_string(const QSize& size) {
    return QString("(width = %1, height = %2)")
        .arg(size.width())
        .arg(size.height());
}
QString to_string(const QSizeF& size) {
    return QString("(width = %1, height = %2)")
        .arg(size.width())
        .arg(size.height());
}

QString to_string(const QPoint& point) {
    return QString("(x = %1, y = %2)").arg(point.x()).arg(point.y());
}

QString to_string(const QPointF& point) {
    return QString("(x = %1, y = %2)").arg(point.x()).arg(point.y());
}

QString to_string(const QRect& rect) {
    return QString("(x = %1, y = %2, width = %3, height = %4)")
        .arg(rect.x())
        .arg(rect.y())
        .arg(rect.width())
        .arg(rect.height());
}

QString to_string(const QRectF& rect) {
    return QString("(x = %1, y = %2, width = %3, height = %4)")
        .arg(rect.x())
        .arg(rect.y())
        .arg(rect.width())
        .arg(rect.height());
}

QString to_string(const QDate& date) { return date.toString("dd.MM.yyyy"); }

QString to_string(const QDateTime& dateTime) {
    return dateTime.toString("dd.MM.yyyy hh:mm:ss.zzz");
}

QString to_string(const QTime& time) { return time.toString("hh:mm:ss.zzz"); }

QString to_string(const QUuid& uuid) { return uuid.toString(); }

QString to_string(const QStringList& list) {
    return QString("[%1]").arg(list.join(", "));
}

QString to_string(const QRegularExpression& expression) {
    return expression.pattern();
}

QString to_string(const QUrl& url) { return url.toString(); }

QString to_string(const QByteArray& byteArray) {
    return QString::fromUtf8(byteArray.toBase64());
}

#endif
