#include "PathItem.h"
#include "QGtifReader.h"
#include <QPainter>

#include "logging.h"

Q_LOGGING_CATEGORY_DEFAULT(PathItem)

PathItem::PathItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , _font(QFont(QStringLiteral("consolas"), DEFALUT_FONT_SIZE))
    , _pen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
{}

void PathItem::updatePoints(QList<QPointF> &&posList)
{
   _posPaths  = QPainterPath();
   _texts.clear();

   if(posList.isEmpty())
       return;

   _posPaths.moveTo(posList.first());


   QFontMetrics fontMetrics(_font);
   int height = fontMetrics.height();
   int width = height * 10;

   for(int i = 1, len = posList.size(); i < len; ++i) {
       auto center = (posList.at(i) + posList.at(i - 1) ) / 2.0;

       _posPaths.moveTo(posList.at(i - 1));
//       if(posList.at(i).y() <  posList.at(i - 1).y())
//           _posPaths.lineTo(QPoint(center.x(), center.y() +  height / 2.0));
//       else
//           _posPaths.lineTo(QPoint(center.x(), center.y() -  height / 2.0));

       auto text = Global::MakeMeterPrintable(qGtifReader->getDistanceMByPoint(posList.at(i), posList.at(i - 1)));

       width = fontMetrics.horizontalAdvance(text);

       auto textRect = QRect(center.x() - width / 2.0, center.y() - height / 2.0, width, height);

       _texts << std::make_tuple(textRect, text);

//       if(posList.at(i).y() <  posList.at(i - 1).y())
//           _posPaths.moveTo(QPoint(center.x(), center.y() -  height / 2.0));
//       else
//           _posPaths.moveTo(QPoint(center.x(), center.y() +  height / 2.0));

       _posPaths.lineTo(posList.at(i));
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
    painter->setPen(_pen);
    painter->drawPath(_posPaths);

    painter->save();
    painter->setPen(Qt::black);
    painter->setFont(_font);

    for(const auto text: _texts) {
        painter->fillRect(std::get<TEXT_POINT>(text), Qt::white);
        painter->drawText(std::get<TEXT_POINT>(text), Qt::AlignCenter, std::get<TEXT_CNTXT>(text));
    }
    painter->restore();

    painter->restore();
}
