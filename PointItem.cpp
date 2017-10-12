#include "PointItem.h"
#include <QPainter>

#include <MapItem.h>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "logging.h"

Q_LOGGING_CATEGORY_DEFAULT(MapItem)

extern QPoint MapToMapView(QPoint);

PointItem::PointItem(MapItem *map, const QPoint &pos, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , _map(map)
    , _pos(pos)
    , _ctrlObj(0)
{
    setFlag(GraphicsItemFlag::ItemIsMovable, true);
    setTransformOriginPoint(_pos);
}

QRectF PointItem::boundingRect() const
{
    return QRect(_pos.x() - 8, _pos.y() - 8, 16, 16);
}

void PointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->fillRect(QRect(_pos.x() - 8, _pos.y() - 8, 16, 16), Qt::red);
    painter->restore();
}

void PointItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(_ctrlObj) _ctrlObj->onItemMoving();
    return QGraphicsItem::mouseMoveEvent(event);
}
