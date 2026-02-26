#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("LogPulse");
    QCoreApplication::setApplicationName("LogPulse");

    QFile styleFile(":/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly))
    {
        QString style = QLatin1String(styleFile.readAll());
        a.setStyleSheet(style);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
