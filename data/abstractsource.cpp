#include "abstractsource.hpp"

#include "abstractdata.hpp"

namespace data
{
    AbstractSource::AbstractSource(QObject* parent) : QObject{parent}
    {
    }
    QSharedDataPointer<AbstractData> AbstractSource::getData(int index) const
    {
        return _data[index];
    }

    AbstractSource::MetaData AbstractSource::getMetaData(int index) const
    {
        return _dataMetaData[index];
    }

    int AbstractSource::getDataCount() const
    {
        return _data.count();
    }
} // namespace data
