#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsItem>

class IClickedOnMap
{
public:
    virtual void onClick(QPointF p) = 0;
};

class MapItem : public QGraphicsItem
{
public:
    MapItem(QGraphicsItem *parent = Q_NULLPTR);

    inline void registClickEvent(IClickedOnMap *obj) { _clickObj = obj; }
    inline void unRegistClickEvent(IClickedOnMap *) { _clickObj = nullptr; }

public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    IClickedOnMap *_clickObj;
};

#endif // MAPITEM_H
