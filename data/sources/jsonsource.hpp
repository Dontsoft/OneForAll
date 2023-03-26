#ifndef DATA_JSONSOURCE_HPP
#define DATA_JSONSOURCE_HPP

#include "data/abstractsource.hpp"

namespace data {

    class JSONSource : public data::AbstractSource
    {
    public:
        explicit JSONSource(QObject* parent = nullptr);
        void read() override;
    };

} // namespace data

#endif // DATA_JSONSOURCE_HPP
