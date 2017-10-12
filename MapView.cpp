#include "MapView.h"
#include "QGtifReader.h"

#include "logging.h"

#include <QResizeEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPointF>
#include <QGestureEvent>
#include <QMessageBox>

#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

Q_LOGGING_CATEGORY_DEFAULT(MapView)

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent),
      _scale(1.0),
      _zoomDelta(0.1),
      _translateSpeed(1.0),
      _bMouseTranslate(false),
      _pathitem(nullptr)
{
    _map = new MapItem();
    _map->registClickEvent(this);
    _scene.addItem(_map);

    _pathitem = new PathItem();
    _pathitem->setParentItem(_map);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCursor(Qt::PointingHandCursor);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);

    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
    setScene(&_scene);

    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);
}

void MapView::setTranslateSpeed(qreal speed)
{
    Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
               "MapView::setTranslateSpeed", "Speed should be in range [0.0, 2.0].");
    _translateSpeed = speed;
}

qreal MapView::translateSpeed() const
{
    return _translateSpeed;
}

void MapView::setZoomDelta(qreal delta)
{
    Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
               "MapView::setZoomDelta", "Delta should be in range [0.0, 1.0].");
    _zoomDelta = delta;

}

qreal MapView::zoomDelta() const
{
    return _zoomDelta;
}


void MapView::mouseMoveEvent(QMouseEvent *event)
{
    if (_bMouseTranslate){
        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(_lastMousePos);
        translateF(mouseDelta);
    }

    _lastMousePos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(!dynamic_cast<PointItem *>(scene()->itemAt(mapToScene(event->pos()), transform())))
            _bMouseTranslate = true;
        _lastMousePos = event->pos();
    } else  if(event->button() == Qt::RightButton) {
        makePointItem(event->pos());
    }

    QGraphicsView::mousePressEvent(event);
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        _bMouseTranslate = false;

    QGraphicsView::mouseReleaseEvent(event);
}

void MapView::wheelEvent(QWheelEvent *event)
{
    QPoint scrollAmount = event->angleDelta();
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void MapView::gestureEvent(QGestureEvent *event)
{
    /// \todo use Gesture
//    if (QGesture *swipe = event->gesture(Qt::SwipeGesture)) {
//        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
//   } else if (QGesture *pan = event->gesture(Qt::PanGesture))
//        panTriggered(static_cast<QPanGesture *>(pan));
//    else if (QGesture *tap = event->gesture(Qt::TapGesture))
//        tapTriggered(static_cast<QTapGesture *>(tap));
//    else if (QGesture *tapAndHold = event->gesture(Qt::TapAndHoldGesture))
//        tapAndHoldTriggered(static_cast<QTapAndHoldGesture *>(tapAndHold));
//    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
//        pinchTriggered(static_cast<QPinchGesture *>(pinch));
}

void MapView::makePointItem(QPoint pos)
{
    auto p = mapToScene(pos).toPoint();
    auto item = scene()->itemAt(p, transform());

    if (item == NULL)
        return;

    PointItem *pitem = dynamic_cast<PointItem *>(item);
    if(pitem) {
        /// delete pitem ?
        if(QMessageBox::warning(this, tr("Delete this"), tr("Delete this ?"), QMessageBox::Yes | QMessageBox::Cancel)
                == QMessageBox::Yes) {
            _pointItems.removeOne(pitem);
            scene()->removeItem(pitem);
            delete pitem;
        }
    } else {
        auto pointItem = new PointItem(_map,std::move(p));
        pointItem->setCtrlObj(this);
        _pointItems.append(pointItem);

        pointItem->setParentItem(_map);
        pointItem->setScale(1.0 / _scale);
    }

    reBuildPath();
    _scene.update();
}

void MapView::reBuildPath()
{
    QList<QPointF> points;
    for(PointItem *item: _pointItems) {
        points << item->getPos();
    }

    _pathitem->updatePoints(std::move(points));
    scene()->update();
}

void MapView::zoomIn()
{
    zoom(1 + _zoomDelta);
}

void MapView::zoomOut()
{
    zoom(1 - _zoomDelta);
}

void MapView::zoom(float scaleFactor)
{
    _scale *= scaleFactor;

    auto oldPos = mapToScene(_lastMousePos);
    scale(scaleFactor, scaleFactor);
    auto newPos = mapToScene(_lastMousePos);

    foreach(QGraphicsItem *item, scene()->items()) {
        if(auto pointitem = dynamic_cast<PointItem *>(item)) {
           pointitem->setScale(1.0 / _scale);
        } else if(auto pathitem = dynamic_cast<PathItem *>(item)) {
            pathitem->setFontScale(1.0 / _scale);
         }
    }

    translateF(newPos - oldPos);
}

void MapView::translateF(QPointF delta)
{
    delta *= _scale;
    delta *= _translateSpeed;

    centerOn(mapToScene(VIEW_WIDTH / 2.0 - delta.x(),  VIEW_HEIGHT / 2.0 - delta.y()));
}

bool MapView::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Gesture:
    {
        gestureEvent(dynamic_cast<QGestureEvent *>(event));
        return true;
    }
    default:
        return QGraphicsView::event(event);
    }

}


void MapView::onClick(QPointF p)
{
    emit GeoCoordinateOnMap(qGtifReader->point2GeoCoordinate(p.toPoint()));
}

void MapView::onItemMoving()
{
    reBuildPath();
}
