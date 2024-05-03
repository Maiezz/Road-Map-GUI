#include "Mainwindow.h"
#include <QMainWindow>

Mainwindow::Mainwindow(QWidget* parent)
    : QMainWindow(parent) , ui(new Ui::MainwindowClass)
{
    ui->setupUi(this);
   
}

Mainwindow::~Mainwindow()
{
    delete ui;
}
