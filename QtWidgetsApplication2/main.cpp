#include "QtApplication1.h"
#include <QtWidgets/QApplication>
#include<iostream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtApplication1 w;

    w.showMaximized();
    return a.exec();
}

