#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //initializes the qt application
    MainWindow w;   //Creates the main widow
    w.show();       //Makes the window visible
    return a.exec();    //keeps the event running
}
