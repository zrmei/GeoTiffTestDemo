#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include "MapItem.h"
#include "PointItem.h"

class QWheelEvent;
class QKeyEvent;

class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapView(QWidget *parent = 0);

    void setTranslateSpeed(qreal speed);
    qreal translateSpeed() const;

    void setZoomDelta(qreal delta);
    qreal zoomDelta() const;

public:
    virtual bool event(QEvent *event) override;

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

public Q_SLOTS:
    void zoomIn();
    void zoomOut();
    void zoom(float scaleFactor);
    void translateF(QPointF delta);

private:
    Qt::MouseButton m_translateButton;
    qreal m_translateSpeed;
    qreal m_zoomDelta;
    bool m_bMouseTranslate;
    QPoint m_lastMousePos;
    qreal m_scale;
    QGraphicsScene _scene;
    MapItem *_map;
    PointItem *_pointItem;
};
#endif // MAPVIEW_H
