#ifndef DATAREADER_HPP
#define DATAREADER_HPP

#include <QObject>
#include <QSharedDataPointer>
#include <QVector>

#include "abstractdata.hpp"

namespace data
{
    class AbstractData;

    class AbstractSource : public QObject
    {
    public:
        struct MetaData
        {
        };

        explicit AbstractSource(QObject* parent = nullptr);

        virtual void read() = 0;

        QSharedDataPointer<AbstractData> getData(int index = 0) const;

        MetaData getMetaData(int index = 0) const;

        int getDataCount() const;

    protected:
        QVector<QSharedDataPointer<AbstractData>> _data;
        QVector<MetaData> _dataMetaData;
    };
} // namespace data

#endif // DATAREADER_HPP
