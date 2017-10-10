#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _mapView = new MapView(ui->mapViewContainer);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    _mapView->resize(event->size());
}
