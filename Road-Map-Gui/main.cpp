#include "QtApplication1.h"
#include "GraphView.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtApplication1 w;
    w.showMaximized();
    return a.exec();
}
