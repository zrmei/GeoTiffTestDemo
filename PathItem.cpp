#include "PathItem.h"
#include "QGtifReader.h"
#include <QPainter>

#include "logging.h"

Q_LOGGING_CATEGORY_DEFAULT(PathItem)

PathItem::PathItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , _font(QFont(QStringLiteral("consolas"), DEFALUT_FONT_SIZE))
{}

void PathItem::updatePoints(QList<QPointF> &&posList)
{
   _posPaths  = QPainterPath();
   _texts.clear();
   _posPaths.moveTo(posList.first());

   for(int i = 1, len = posList.size(); i < len; ++i) {
       _posPaths.lineTo(posList.at(i));
       auto center = (posList.at(i) + posList.at(i - 1) ) / 2.0;
       _texts << std::make_tuple(center, QStringLiteral("%1m").arg(qGtifReader->getDistanceMByPoint(posList.at(i), posList.at(i - 1)), 0, 'f', 2));
       _posPaths.moveTo(posList.at(i));
   }
}

QRectF PathItem::boundingRect() const
{
    return _posPaths.boundingRect();
}

#define TEXT_POINT 0
#define TEXT_CNTXT 1

void PathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setPen(Qt::red);
    painter->drawPath(_posPaths);

    painter->save();
    painter->setPen(Qt::blue);
    painter->setFont(_font);

    for(const auto text: _texts) {
        painter->drawText(std::get<TEXT_POINT>(text), std::get<TEXT_CNTXT>(text));
    }
    painter->restore();

    painter->restore();
}
