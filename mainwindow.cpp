#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QResizeEvent>
#include <QFileDialog>
#include <QStandardPaths>

#include "QGtifReader.h"

#include <QSettings>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _mapView = new MapView(ui->mapViewContainer);

    connect(_mapView, &MapView::GeoCoordinateOnMap, this, &MainWindow::onClickedOnMap);
    connect(qGtifReader, &QGtifReader::fileChanged, _mapView, QOverload<void>::of(&MapView::update));
}

MainWindow::~MainWindow()
{
    delete ui;
}

enum class Coordinate {
    Latitude,
    Longitude
};

static QString MakeDegPrintable( double dfAngle, Coordinate type )

{
    static QString szBuffer;
    const char	*pszHemisphere = NULL;

    double dfRound = 0.5 / 60 * 0.01;

    int nDegrees = (int) fabs(dfAngle);
    int nMinutes = (int) ((fabs(dfAngle) - nDegrees) * 60 + dfRound);
    double dfSeconds = fabs((fabs(dfAngle) * 3600 - nDegrees*3600 - nMinutes*60));

    if( type == Coordinate::Longitude && dfAngle < 0.0 )
        pszHemisphere = "W";
    else if( type == Coordinate::Longitude )
        pszHemisphere = "E";
    else if( dfAngle < 0.0 )
        pszHemisphere = "S";
    else
        pszHemisphere = "N";

    szBuffer.sprintf("%2dd %2d\'%.3f\" %s", nDegrees, nMinutes, dfSeconds, pszHemisphere);

    return szBuffer;
}

void MainWindow::onClickedOnMap(QGeoCoordinate coord)
{
    ui->lblLatitude->setText(MakeDegPrintable(coord.latitude(), Coordinate::Latitude));
    ui->lblLongitude->setText(MakeDegPrintable(coord.longitude(), Coordinate::Longitude));
    ui->lblAltitude->setText(QString::number(coord.altitude()));
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    _mapView->resize(ui->mapViewContainer->size());
}

void MainWindow::on_pushButton_clicked()
{
    QString lastpath = Global::loadSettings(QStringLiteral("lastpath"), QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0)).toString();

    QString path = QFileDialog::getOpenFileName(this, tr("Select tif"), lastpath, "*.tif");

    if(path.isEmpty())
        return;

    Global::saveSettings(QStringLiteral("lastpath"),  QFileInfo(path).canonicalPath());

    if(!qGtifReader->setFilePath(path)) {
        QMessageBox::critical(this, tr("Error"), tr("File format error"));
        return;
    }
}
