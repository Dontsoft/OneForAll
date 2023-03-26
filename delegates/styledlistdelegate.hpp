#ifndef STYLEDLISTDELEGATE_HPP
#define STYLEDLISTDELEGATE_HPP

#include <QStyledItemDelegate>

class StyledListDelegate : public QStyledItemDelegate
{
public:
    StyledListDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};

#endif // STYLEDLISTDELEGATE_HPP
