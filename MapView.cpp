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

#define isPointAtItem(pos, Item) dynamic_cast<Item *>(scene()->itemAt(pos, transform()))
#define isPointAtMap(pos)        isPointAtItem(pos, MapItem)
#define isPointAtPointItem(pos)  isPointAtItem(pos, PointItem)

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent),
      _scale(1.0),
      _zoomDelta(0.1),
      _translateSpeed(1.0),
      _bMouseTranslate(false),
      _pathitem(nullptr)
{
    _map = new MapItem();
    _scene.addItem(_map);

    _pathitem = new PathItem();
    _pathitem->setParentItem(_map);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCursor(Qt::ArrowCursor);
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
    QPointF pointAtMap = mapToScene(event->pos());
\
    if (_bMouseTranslate){
        if(isPointAtMap(pointAtMap))
            setCursor(Qt::ClosedHandCursor);
        else
            setCursor(Qt::ArrowCursor);

        QPointF mouseDelta = pointAtMap - _lastImgPos;
        translateF(mouseDelta);
    } else if(isPointAtMap(pointAtMap)) {
        setCursor(Qt::CrossCursor);
        auto coord = qGtifReader->point2GeoCoordinate(pointAtMap.toPoint());
        emit GeoCoordinateOnMap(coord);
        showToolTip(coord);
    } else {
        setCursor(Qt::ArrowCursor);
    }

    _lastMousePos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(!isPointAtPointItem(mapToScene(event->pos())))
            _bMouseTranslate = true;

        _lastMousePos = event->pos();
        _lastImgPos = mapToScene(event->pos());
    } else if(event->button() == Qt::RightButton) {
        makePointItem(event->pos());
    }

    QGraphicsView::mousePressEvent(event);
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        _bMouseTranslate = false;
        setCursor(Qt::CrossCursor);
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void MapView::wheelEvent(QWheelEvent *event)
{
    setCursor(Qt::BusyCursor);
    QPoint scrollAmount = event->angleDelta();
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
    setCursor(Qt::CrossCursor);
}

void MapView::gestureEvent(QGestureEvent *event)
{
    /// \todo use Gesture
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
        _pointItems.append(pointItem);
        pointItem->setCtrlObj(this);
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

    emit pathBuildFinished(points);
}

void MapView::showToolTip(const QGeoCoordinate &Coord)
{
    using namespace Global;
    setToolTip(QStringLiteral("Latitude: %1\nLongitude: %2\nAltitude: %3")
               .arg(MakeGeoPrintable(Coord.latitude(), GeoType::Latitude))
               .arg(MakeGeoPrintable(Coord.longitude(),GeoType::Longitude))
               .arg(MakeMeterPrintable(Coord.altitude()))
               );
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
            reBuildPath();
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

void MapView::resloveView()
{
    QImage map = qGtifReader->getMap();
    float scaleFactor = qMin(VIEW_WIDTH, VIEW_HEIGHT) * 1.0 / qMax(map.width(), map.height());

    /// reset and then scale
    zoom(1.0 / _scale * scaleFactor);

    removeAllPathPoint();

    centerOn(map.width() / 2, map.height() /2);
    update();
}

void MapView::removePathPointAt(int idx)
{
    /// two points, delete last one
    ++idx;

    if(_pointItems.isEmpty() || idx > _pointItems.size())
        return;

    auto item = _pointItems.takeAt(idx);
    scene()->removeItem(item);
    delete item;

    /// delete all where only two points
    if(_pointItems.size() == 1) {
        auto item = _pointItems.takeAt(0);
        scene()->removeItem(item);
        delete item;
    }

    reBuildPath();
}

void MapView::removeAllPathPoint()
{
    for(PointItem *item: _pointItems) {
        scene()->removeItem(item);
        delete item;
    }

    _pointItems.clear();
    reBuildPath();
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

void MapView::onItemMoving()
{
    reBuildPath();
}
