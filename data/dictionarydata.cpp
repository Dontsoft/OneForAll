#include "dictionarydata.hpp"

namespace data
{

    namespace implementation
    {

        // Node

        inline bool Node::isObject() const
        {
            return false;
        }

        inline bool Node::isArray() const
        {
            return false;
        }

        inline bool Node::isValue() const
        {
            return false;
        }

        Node* Node::child(int index) const
        {
            return nullptr;
        }

        Node* Node::child(const QString& key) const
        {
            return nullptr;
        }

        // Object Node

        Node* ObjectNode::child(int index) const
        {
            return children.values()[index];
        }

        Node* ObjectNode::child(const QString& key) const
        {
            return children.value(key);
        }

        inline bool ObjectNode::isObject() const
        {
            return true;
        }

        QVariant ObjectNode::value() const
        {
            return QVariant();
        }

        // Array Node

        inline bool ArrayNode::isArray() const
        {
            return true;
        }

        Node* ArrayNode::child(int index) const
        {
            return children[index];
        }

        QVariant ArrayNode::value() const
        {
            return QVariant();
        }

        // Value Node

        inline bool ValueNode::isValue() const
        {
            return true;
        }

        QVariant ValueNode::value() const
        {
            return _value;
        }

    } // namespace implementation

    DictionaryData::DictionaryData()
    {

    }

} // namespace data
