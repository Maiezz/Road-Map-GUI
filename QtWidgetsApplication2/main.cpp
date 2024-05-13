#include "QtApplication1.h"
#include "CountryGraph.h"
#include <QtWidgets/QApplication>
#include<iostream>
#include <GraphView.h>



int main(int argc, char* argv[])
{




	QApplication a(argc, argv);

	GraphViewClass w;

	w.showFullScreen();

	// QtApplication1 w;

	// w.showMaximized();

	return a.exec();
}

