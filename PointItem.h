#ifndef POINTITEM_H
#define POINTITEM_H
#include <QGraphicsItem>

class PointItem : public QGraphicsItem
{
public:
    PointItem(QGraphicsItem *parent = Q_NULLPTR);

public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // POINTITEM_H
