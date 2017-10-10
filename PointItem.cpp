#include "PointItem.h"
#include <QPainter>

PointItem::PointItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{

}

QRectF PointItem::boundingRect() const
{
    return QRect(-20, -20, 20, 20);
}

void PointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->drawRect(QRect(-20, -20, 20, 20));
    painter->restore();
}



