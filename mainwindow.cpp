#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QResizeEvent>
#include <QFileDialog>
#include <QStandardPaths>

#include "QGtifReader.h"

#include <QSettings>
#include <QMessageBox>
#include <QMenu>

#include "logging.h"

Q_LOGGING_CATEGORY_DEFAULT(MainWindow)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lblLogo->setStyleSheet(QStringLiteral("background-color : white;"));
    connect(ui->lwPoints, &QListWidget::customContextMenuRequested, this, &MainWindow::onCustomContextMenuRequested);

    _mapView = new MapView(ui->mapViewContainer);

    connect(_mapView, &MapView::GeoCoordinateOnMap, this, &MainWindow::onClickedOnMap);
    connect(_mapView, &MapView::pathBuildFinished,  this, &MainWindow::onPathBuildFinished);

    connect(qGtifReader, &QGtifReader::fileChanged, this, &MainWindow::onTifFileChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClickedOnMap(QGeoCoordinate coord)
{
    using namespace Global;
    ui->lblLatitude->setText(MakeGeoPrintable(coord.latitude(), GeoType::Latitude));
    ui->lblLongitude->setText(MakeGeoPrintable(coord.longitude(),GeoType::Longitude));
    ui->lblAltitude->setText(MakeMeterPrintable(coord.altitude()));
}

void MainWindow::onTifFileChanged()
{
    ui->lblMapCoordType->setText(qGtifReader->getPCSInfo());
    _mapView->resloveView();
}

void MainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    auto _currentItem = static_cast<QListWidgetItem *>(ui->lwPoints->itemAt(pos));

    QMenu menu(ui->lwPoints);
    if(_currentItem){
        QAction *actRemove = new QAction(tr("&Remove"), &menu);
        int index = ui->lwPoints->currentRow();

        connect(actRemove, &QAction::triggered, [this, index]() { _mapView->removePathPointAt(index); });

        menu.addAction(actRemove);
    }

    QAction *newAct = new QAction(tr("&Delete All"), &menu);
    connect(newAct, &QAction::triggered, _mapView, &MapView::removeAllPathPoint);

    menu.addAction(newAct);

    menu.exec(ui->lwPoints->mapToGlobal(pos));
}

void MainWindow::onPathBuildFinished(QList<QPointF> points)
{
    static QString format = QStringLiteral("%1 : %2");

    QStringList labels;
    for(int i = 1, len = points.size(); i < len; ++i) {
        auto dist = qGtifReader->getDistanceMByPoint(points.at(i), points.at(i - 1));
        auto text = format.arg(i).arg(Global::MakeMeterPrintable(dist));
        labels << text;
    }

    ui->lwPoints->clear();
    if(points.isEmpty())
        return;

    ui->lwPoints->addItems(labels);
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    _mapView->resize(ui->mapViewContainer->size());
    ui->lblLogo->setPixmap(QPixmap(":/img/logo.png").scaled(ui->lblLogo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::on_pushButton_clicked()
{
    QString lastpath = Global::loadSettings(
                QStringLiteral("lastpath"),
                QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0)
                ).toString();

    QString path = QFileDialog::getOpenFileName(this, tr("Select tif"), lastpath, QStringLiteral("*.tif"));

    if(path.isEmpty())
        return;

    Global::saveSettings(QStringLiteral("lastpath"),  QFileInfo(path).canonicalPath());

    if(!qGtifReader->setFilePath(path)) {
        QMessageBox::critical(this, tr("Error"), tr("File format error"));
        return;
    }

    setWindowTitle(path);
}
