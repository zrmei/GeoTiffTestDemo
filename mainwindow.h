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
    virtual void resizeEvent(QResizeEvent *) override;

private slots:
    void onClickedOnMap(QGeoCoordinate coord);
    void onTifFileChanged();
    void onCustomContextMenuRequested(const QPoint &pos);
    void onPathBuildFinished(QList<QPointF> points);
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    MapView *_mapView;
    QList<QPointF> _points;
};

#endif // MAINWINDOW_H
