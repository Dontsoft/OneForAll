#ifndef DATAREADER_HPP
#define DATAREADER_HPP

#include <QSharedDataPointer>

namespace data
{
    class AbstractData;

    class AbstractSource
    {
    public:
        AbstractSource();

        virtual void read() = 0;

        QSharedDataPointer<AbstractData> getData() const
        {
            return _data;
        }

    protected:
        QSharedDataPointer<AbstractData> _data;
    };
} // namespace data

#endif // DATAREADER_HPP
