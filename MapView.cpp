#include "MapView.h"
#include "QGtifReader.h"
#include <QResizeEvent>
#include <QMouseEvent>

#include <QWheelEvent>
#include <QKeyEvent>
#include <QPointF>

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

#include "logging.h"

Q_LOGGING_CATEGORY_DEFAULT(MapView)

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent),
      m_translateButton(Qt::LeftButton),
      m_scale(1.0),
      m_zoomDelta(0.1),
      m_translateSpeed(1.0),
      m_bMouseTranslate(false)
{
    _map = new MapItem();
    _scene.addItem(_map);

    _pointItem = new PointItem();
    _scene.addItem(_pointItem);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCursor(Qt::PointingHandCursor);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);

    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
    setScene(&_scene);
    centerOn(mapToScene(464, 317));

    setAttribute(Qt::WA_AcceptTouchEvents, true);
}


void MapView::setTranslateSpeed(qreal speed)
{
    Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
               "MapView::setTranslateSpeed", "Speed should be in range [0.0, 2.0].");
    m_translateSpeed = speed;
}

qreal MapView::translateSpeed() const
{
    return m_translateSpeed;
}

void MapView::setZoomDelta(qreal delta)
{
    Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
               "MapView::setZoomDelta", "Delta should be in range [0.0, 1.0].");
    m_zoomDelta = delta;

}

qreal MapView::zoomDelta() const
{
    return m_zoomDelta;
}


void MapView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMouseTranslate){
        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(m_lastMousePos);
        translateF(mouseDelta);
    }

    m_lastMousePos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton) {
        m_bMouseTranslate = true;
        m_lastMousePos = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton)
        m_bMouseTranslate = false;

    QGraphicsView::mouseReleaseEvent(event);
}

void MapView::resizeEvent(QResizeEvent *event)
{
}


void MapView::wheelEvent(QWheelEvent *event)
{
    QPoint scrollAmount = event->angleDelta();
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void MapView::zoomIn()
{
    zoom(1 + m_zoomDelta);
}

void MapView::zoomOut()
{
    zoom(1 - m_zoomDelta);
}

void MapView::zoom(float scaleFactor)
{
    m_scale *= scaleFactor;

    auto oldPos = mapToScene(m_lastMousePos);
    scale(scaleFactor, scaleFactor);
    auto newPos = mapToScene(m_lastMousePos);

    foreach(QGraphicsItem *item, scene()->items()) {
        if(dynamic_cast<MapItem *>(item)) {
            continue;
        }

        item->setScale(1.0 / m_scale);
    }

    translateF(newPos - oldPos);
}

void MapView::translateF(QPointF delta)
{
    delta *= m_scale;
    delta *= m_translateSpeed;

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

bool MapView::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    {
        LOG_DEBUG() << "touch Begin";
        return true;
    }
    case QEvent::TouchUpdate:
    {
        LOG_DEBUG() << "touch Update";
        return true;
    }
    case QEvent::TouchEnd:
    {
        LOG_DEBUG() << "touch end";
        return true;
    }
    default:
        return QGraphicsView::event(event);
    }

}
