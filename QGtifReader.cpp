#include "QGtifReader.h"

QGtifReader::QGtifReader(const QString &filepath, QObject *parent)
    : QObject(parent)
    , _map(QImage(filepath))
    , _tif(nullptr)
    , _gtif(nullptr)
{
    _tif = XTIFFOpen(filepath.toStdString().c_str(), "r");
    if(_tif)
        _gtif = GTIFNew(_tif);
}

QGtifReader::~QGtifReader()
{
    if(_gtif) GTIFFree(_gtif);
    if(_tif) XTIFFClose(_tif);;
}

bool QGtifReader::isVaild() const
{
    return _tif && _gtif;
}

auto QGtifReader::point2GeoCoordinate(const QPoint &pos) const -> QGeoCoordinate
{
    return QGeoCoordinate();
}

auto QGtifReader::geoCoordinate2Point(const QGeoCoordinate &coord) const -> QPoint
{
    return QPoint();
}

auto QGtifReader::dragTo(const QPoint &pos) -> void
{

}

auto QGtifReader::getMap() const -> QImage
{
    return QImage();
}

auto QGtifReader::_buildMap() -> void
{

}

auto QGtifReader::_loadSettings() -> void
{

}

auto QGtifReader::_saveSettings() -> void
{

}

void QGtifReader::setZoom(double zoom, const QPoint &center)
{
    _zoom = zoom; _curMouseCenter = center;

    _scaledMap = _map.scaled(_map.width() * zoom, _map.height() * zoom);

    _buildMap();
}
