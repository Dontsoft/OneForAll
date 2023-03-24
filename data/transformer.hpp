#ifndef DATA_TRANSFORMER_HPP
#define DATA_TRANSFORMER_HPP

#include "dictionarydata.hpp"
#include "tabulardata.hpp"

namespace data {

    class Transformer
    {
    public:
        Transformer();
        QSharedDataPointer<DictionaryData> toDictionaryData(
            QSharedDataPointer<TabularData> data) const;
        QSharedDataPointer<TabularData> toTabularData(
            QSharedDataPointer<DictionaryData> data) const;
    };

} // namespace data

#endif // DATA_TRANSFORMER_HPP
