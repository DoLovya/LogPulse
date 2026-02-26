#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("LogPulse");
	QCoreApplication::setApplicationName("LogPulse");

	// Load Font
	int fontId = QFontDatabase::addApplicationFont(":/fonts/AlibabaPuHuiTi375SemiBold-Regular.otf");
	if (fontId != -1) {
		QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
		if (!fontFamilies.isEmpty()) {
			QString fontFamily = fontFamilies.first();
			QFont font(fontFamily);
			font.setPointSizeF(10);
			a.setFont(font);
		}
	}

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
