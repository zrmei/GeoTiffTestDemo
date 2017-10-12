#ifndef PATHITEM_H
#define PATHITEM_H

#include <QGraphicsItem>
#include <QFont>

#define DEFALUT_FONT_SIZE 32
class MapItem;
class PathItem : public QGraphicsItem
{
public:
    PathItem(QGraphicsItem *parent = Q_NULLPTR);

    void updatePoints(QList<QPointF> &&posList);

    void setFontScale(double scale) { _font.setPointSizeF(scale * DEFALUT_FONT_SIZE); }

public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPainterPath _posPaths;
    QFont _font;
    QList<std::tuple<QPointF, QString>> _texts;

};

#endif // PATHITEM_H
