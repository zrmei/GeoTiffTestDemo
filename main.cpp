#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(QStringLiteral(APPLICATION_NAME));
    a.setApplicationVersion(QStringLiteral(APPLICATION_VERSION));
    a.setOrganizationName(QStringLiteral(APPLICATION_NAME));
    a.setOrganizationDomain(QStringLiteral("org." APPLICATION_NAME));

    a.setAttribute(Qt::AA_EnableHighDpiScaling, true);
    a.setAttribute(Qt::AA_Use96Dpi, true);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}

