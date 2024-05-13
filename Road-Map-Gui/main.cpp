#include "QtApplication1.h"
#include "GraphView.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphViewClass w;
    w.show();
    return a.exec();
}
