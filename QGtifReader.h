#ifndef QGTIFREADER_H
#define QGTIFREADER_H

#include <QObject>
#include <QImage>
#include <QGeoCoordinate>
#include <QPoint>

#include "xtiffio.h"  /* for TIFF */
#include "geotiffio.h" /* for GeoTIFF */

#include "global.h"

class QGtifReader: public QObject
{
    Q_OBJECT
signals:
    void MapChanged();
    void zoomChanged(double zoom);

public:
    Q_DECLARE_STATIC_INSTANCE(QGtifReader);

    auto setFilePath(const QString &filepath) -> void;
    auto setRect(const QRect &rect) ->void;
    auto isVaild() const -> bool;

public:
    auto setZoom(double zoom, const QPoint &center = QPoint()) -> void;
    inline auto getZoom() const -> double;

    auto point2GeoCoordinate(const QPoint &pos) const -> QGeoCoordinate;
    auto geoCoordinate2Point(const QGeoCoordinate &coord) const -> QPoint;

    auto dragTo(const QPoint &pos) -> void;

    auto getMap() const -> QImage;

private:
    QGtifReader(QObject *parent = nullptr);
    ~QGtifReader();

    auto _buildMap() -> void;

    auto _loadSettings() -> void;
    auto _saveSettings() -> void;

private:
    double _zoom;
    QPoint _curMouseCenter;
    QImage _map;
    QImage _scaledMap;
    QRect  _rect;

    TIFF *_tif;  /* TIFF-level descriptor */
    GTIF *_gtif; /* GeoKey-level descriptor */

};
#define qGtifReader (QGtifReader::Instance())

auto QGtifReader::getZoom() const -> double
{
    return _zoom;
}


#endif // QGTIFREADER_H
