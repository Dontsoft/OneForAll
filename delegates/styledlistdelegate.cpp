#include "styledlistdelegate.hpp"

StyledListDelegate::StyledListDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

void StyledListDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    auto _option = option;
    _option.rect = option.rect.adjusted(4, 2, -4, -2);
    return QStyledItemDelegate::paint(painter, _option, index);
}
