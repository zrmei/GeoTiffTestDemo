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

class MapView : public QGraphicsView, public IItemMoving
{
    Q_OBJECT
signals:
    void GeoCoordinateOnMap(QGeoCoordinate);
    void pathBuildFinished(QList<QPointF>);

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
    void resloveView();

    void removePathPointAt(int idx);
    void removeAllPathPoint();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    virtual void onItemMoving() override;

protected:
    void gestureEvent(QGestureEvent *event);

private:
    void makePointItem(QPoint pos);
    void reBuildPath();
    void showToolTip(const QGeoCoordinate &);

private:
    qreal            _translateSpeed;
    qreal            _zoomDelta;
    bool             _bMouseTranslate;
    QPoint           _lastMousePos;
    QPointF          _lastImgPos;
    qreal            _scale;
    QGraphicsScene   _scene;
    MapItem          *_map;
    PathItem         *_pathitem;
    QList<PointItem*> _pointItems;
};
#endif // MAPVIEW_H
