#include "MapItem.h"

#include "QGtifReader.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsSceneMouseEvent>

#include "logging.h"

Q_LOGGING_CATEGORY_DEFAULT(MapItem)

MapItem::MapItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , _clickObj(nullptr)
{}

QRectF MapItem::boundingRect() const
{
   auto map = qGtifReader->getMap();
   return QRect(0, 0, map.width(), map.height());
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setPen(Qt::red);
    auto map = qGtifReader->getMap();
    painter->drawImage(QRect(0, 0, map.width(), map.height()), map);
    painter->restore();
}


void MapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(_clickObj) _clickObj->onClick(event->pos());
}
