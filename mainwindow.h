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

private:
    Ui::MainWindow *ui;
    MapView *_mapView;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
