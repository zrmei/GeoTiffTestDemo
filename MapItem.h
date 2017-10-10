#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsItem>


class MapItem : public QGraphicsItem
{
public:
    MapItem(QGraphicsItem *parent = Q_NULLPTR);

public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    QRect _rect;

    // QGraphicsItem interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MAPITEM_H
