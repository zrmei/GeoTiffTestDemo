#ifndef PATHITEM_H
#define PATHITEM_H

#include <QGraphicsItem>
#include <QFont>
#include <QPen>

#define DEFALUT_FONT_SIZE 18
#define DEFALUT_PEN_SIZE 2
class MapItem;
class PathItem : public QGraphicsItem
{
public:
    PathItem(QGraphicsItem *parent = Q_NULLPTR);

    void updatePoints(QList<QPointF> &&posList = QList<QPointF>());

    inline void setFontScale(double scale);

public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPainterPath _posPaths;
    QFont _font;
    QList<std::tuple<QRect, QString>> _texts;
    QPen _pen;

};

void PathItem::setFontScale(double scale)
{
    _font.setPointSizeF(scale * DEFALUT_FONT_SIZE);
    _pen.setWidth(scale * DEFALUT_PEN_SIZE);
}

#endif // PATHITEM_H
