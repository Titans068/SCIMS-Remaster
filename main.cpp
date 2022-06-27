#include "mainwindow.h"
#include "login.h"

#include <QApplication>
#include <QDebug>
#include <QFile>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	//qDebug()<<QCoreApplication::applicationDirPath();
	QFile styleFile(QCoreApplication::applicationDirPath() + "//style.qss");
	styleFile.open(QFile::ReadOnly);
	QString style(styleFile.readAll());
	a.setStyleSheet(style);

    Login d;
    if (d.exec())
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
}
