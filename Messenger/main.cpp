#include "mainwindow.h"
#include "loginwidget.h"
#include "writemessegewidget.h"
#include "messegeswidget.h"
#include "requestwidget.h"
#include "clientserver.h"

#include <QApplication>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWind;
    ClientServer server{&mainWind};
    mainWind.setServer(&server);

    mainWind.show();




    return a.exec();
}
