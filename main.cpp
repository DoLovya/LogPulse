#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QCoreApplication::setOrganizationName("LogPulse");
    QCoreApplication::setApplicationName("LogPulse");

    MainWindow w;
    w.show();
    return a.exec();
}
