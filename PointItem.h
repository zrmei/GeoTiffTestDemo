#ifndef POINTITEM_H
#define POINTITEM_H
#include <QGraphicsItem>
#include <QPoint>

class MapItem;

class IItemMoving
{
public:
    virtual void onItemMoving() = 0;
};

class PointItem : public QGraphicsItem
{
public:
    PointItem(MapItem *map, const QPoint &pos, QGraphicsItem *parent = Q_NULLPTR);

    auto getPos() -> QPointF { return pos() + _pos; }
    void setCtrlObj(IItemMoving *obj) { _ctrlObj = obj;}

public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // QGraphicsItem interface
protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    MapItem *_map;
    IItemMoving *_ctrlObj;
    QPoint _pos;
};

#endif // POINTITEM_H
