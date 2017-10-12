#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include "MapItem.h"
#include "PointItem.h"
#include "PathItem.h"
#include <QGeoCoordinate>

class QWheelEvent;
class QKeyEvent;
class QGestureEvent;

class MapView : public QGraphicsView, public IClickedOnMap, public IItemMoving
{
    Q_OBJECT
signals:
    void GeoCoordinateOnMap(QGeoCoordinate);

public:
    explicit MapView(QWidget *parent = 0);

    void setTranslateSpeed(qreal speed);
    qreal translateSpeed() const;

    void setZoomDelta(qreal delta);
    qreal zoomDelta() const;

public:
    virtual bool event(QEvent *event) override;

public Q_SLOTS:
    void zoomIn();
    void zoomOut();
    void zoom(float scaleFactor);
    void translateF(QPointF delta);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    virtual void onClick(QPointF p) override;
    virtual void onItemMoving() override;

protected:
    void gestureEvent(QGestureEvent *event);

private:
    void makePointItem(QPoint pos);
    void reBuildPath();

private:
    qreal            _translateSpeed;
    qreal            _zoomDelta;
    bool             _bMouseTranslate;
    QPoint           _lastMousePos;
    qreal            _scale;
    QGraphicsScene   _scene;
    MapItem          *_map;
    PathItem         *_pathitem;
    QList<PointItem*> _pointItems;
};
#endif // MAPVIEW_H
