#ifndef DATA_DICTIONARYDATA_HPP
#define DATA_DICTIONARYDATA_HPP

#include "abstractdata.hpp"

#include <QMap>
#include <QString>
#include <QVariant>

namespace data
{
    namespace implementation
    {
        struct Node
        {
            QString name;
            QMap<QString, QVariant> attributes;

            virtual inline bool isObject() const;
            virtual inline bool isArray() const;
            virtual inline bool isValue() const;

            virtual Node* child(int index) const;
            virtual Node* child(const QString& key) const;

            virtual QVariant value() const = 0;
            template <typename T> T value() const
            {
                return qvariant_cast<T>(value());
            }
        };

        struct ObjectNode : public Node
        {
            QMap<QString, Node*> children;

            inline bool isObject() const override;

            Node* child(int index) const override;
            Node* child(const QString& key) const override;

            QVariant value() const override;
        };

        struct ArrayNode : public Node
        {
            QVector<Node*> children;

            inline bool isArray() const override;

            virtual Node* child(int index) const override;

            QVariant value() const override;
        };

        struct ValueNode : public Node
        {
            QVariant _value;

            inline bool isValue() const override;

            QVariant value() const override;
        };

    } // namespace implementation

    class DictionaryData : public data::AbstractData
    {
    public:
        DictionaryData();

    private:
        std::unique_ptr<implementation::Node> _root;
    };

} // namespace data

#endif // DATA_DICTIONARYDATA_HPP
