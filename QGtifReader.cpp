#include "QGtifReader.h"

#include "geotiff.h"
#include "xtiffio.h"
#include "geo_normalize.h"
#include "geo_simpletags.h"
#include "geovalues.h"
#include "tiffio.h"
#include "cpl_serv.h"
#include "projects.h"

#include "logging.h"

Q_LOGGING_CATEGORY_DEFAULT(QGtifReader)
Q_STATIC_INSTANCE(QGtifReader)

static const char *CSVFileOverride( const char * pszInput )

{
    static char szPath[1024];

#ifdef WIN32
    sprintf( szPath, "%s\\%s", "csv", pszInput );
#else
    sprintf( szPath, "%s/%s", "csv", pszInput );
#endif

    return ( szPath );
}

QGtifReader::QGtifReader(QObject *parent)
    : QObject(parent)
    , _tif(nullptr)
    , _gtif(nullptr)
    , _defn(new GTIFDefn{})
{
    SetCSVFilenameHook(CSVFileOverride);
}


static auto _point2GeoCoordinate( GTIF *gtif, GTIFDefn *defn, double x, double y) -> QGeoCoordinate
{
    double z = 0;
    /* Try to transform the coordinate into PCS space */
    if( !GTIFImageToPCS( gtif, &x, &y ) )
        return QGeoCoordinate(0, 0, 0);

    if(defn->Model != ModelTypeGeographic) {
        projPJ pj_merc, pj_latlong;

        if (!(pj_merc = pj_init_plus(GTIFGetProj4Defn(defn))))
            return QGeoCoordinate(0, 0, 0);

        if(!(pj_latlong = pj_init_plus("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs")))
            return QGeoCoordinate(0, 0, 0);

        pj_transform(pj_merc, pj_latlong, 1, 1, &x, &y, &z);

        x *= RAD_TO_DEG;
        y *= RAD_TO_DEG;

        pj_free(pj_merc);
        pj_free(pj_latlong);
    }

    return QGeoCoordinate(y, x, z);
}

static auto _geoCoordinate2Point( GTIF *gtif, GTIFDefn *defn, double x, double y) -> QPoint
{
    /// \todo from deg to point
    return QPoint(0, 0);
}

auto QGtifReader::setFilePath(const QString &filepath) -> bool
{
    if(_gtif) { GTIFFree(_gtif); _gtif = nullptr; }
    if(_tif) { XTIFFClose(_tif); _tif = nullptr; }

    if(!(_tif = XTIFFOpen(filepath.toStdString().c_str(), "r")))
        return false;

    if(!(_gtif = GTIFNew(_tif)))
        return false;

    _map.load(filepath);

    GTIFGetDefn(_gtif, _defn);

    emit fileChanged();
    return true;
}

QGtifReader::~QGtifReader()
{
    delete _defn;
    if(_gtif) GTIFFree(_gtif);
    if(_tif) XTIFFClose(_tif);
}

auto QGtifReader::point2GeoCoordinate(const QPoint &pos) const -> QGeoCoordinate
{
    if(!isVaild())
        return QGeoCoordinate(0, 0, 0);

    return _point2GeoCoordinate(_gtif, _defn, pos.x(), pos.y());
}

auto QGtifReader::geoCoordinate2Point(const QGeoCoordinate &coord) const -> QPoint
{
    if(!isVaild())
        return QPoint(0, 0);

    return _geoCoordinate2Point(_gtif, _defn, coord.latitude(), coord.longitude());
}

auto QGtifReader::getDistanceM(const QGeoCoordinate &lh, const QGeoCoordinate &rh) const -> double
{
    static const auto EarthRadiusmm = 6378137.0;

    auto dLat1InRad  = lh.latitude()  * DEG_TO_RAD;
    auto dLong1InRad = lh.longitude() * DEG_TO_RAD;
    auto dLat2InRad  = rh.latitude()  * DEG_TO_RAD;
    auto dLong2InRad = rh.longitude() * DEG_TO_RAD;

    auto dLongitude = dLong2InRad - dLong1InRad;
    auto dLatitude  = dLat2InRad - dLat1InRad;
    auto a = pow(sin(dLatitude / 2), 2) + cos(dLat1InRad) * cos(dLat2InRad) * pow(sin(dLongitude / 2), 2);
    auto c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EarthRadiusmm * c;
}

auto QGtifReader::getDistanceMByPoint(const QPointF &x, const QPointF &y) const -> double
{
    return getDistanceM(point2GeoCoordinate(x.toPoint()), point2GeoCoordinate(y.toPoint()));
}

auto QGtifReader::getPCSInfo() const -> QString
{
    if(!isVaild())
        return QString();

    char *ppszEPSGName = nullptr;
    GTIFGetPCSInfo(_defn->PCS, &ppszEPSGName, NULL, NULL, NULL);

    if(ppszEPSGName == nullptr || strlen(ppszEPSGName) == 0)
        return tr("No longitude and latitude found!");

    return QStringLiteral("PCS: %1 (%2)").arg(_defn->PCS).arg(ppszEPSGName);
}
