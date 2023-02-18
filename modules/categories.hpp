#ifndef CATEGORIES_HPP
#define CATEGORIES_HPP

#include <QObject>

struct Categories
{
    static const QString GENERATORS;
    static const QString DATA_TRANSFORMATION;
    static const QString REGEX;
    static const QString WORKDAY;
    static const QString XML;
};

const QString Categories::GENERATORS = QObject::tr("Generators");
const QString Categories::DATA_TRANSFORMATION
    = QObject::tr("Data Transformation");
const QString Categories::REGEX = QObject::tr("RegEx");
const QString Categories::WORKDAY = QObject::tr("Workday");
const QString Categories::XML = QObject::tr("XML");

#endif // CATEGORIES_HPP
