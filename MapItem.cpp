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
{
    if(!qGtifReader->isVaild())
        qGtifReader->setFilePath(QStringLiteral(R"(D:\Program\downloads\ortho-gk-2mm_optimized_1cm_8bit.tif)"));
}

QRectF MapItem::boundingRect() const
{
   return _rect;
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->setBackground(QBrush(Qt::black));
    auto map = qGtifReader->getMap();
    _rect = QRect(0, 0, map.width(), map.height());
    painter->drawImage(_rect, qGtifReader->getMap());
    painter->restore();
}


void MapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    LOG_DEBUG() << event->pos();
}
