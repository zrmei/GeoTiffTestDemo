#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MapView.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void onClickedOnMap(QGeoCoordinate coord);
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    MapView *_mapView;
};

#endif // MAINWINDOW_H
