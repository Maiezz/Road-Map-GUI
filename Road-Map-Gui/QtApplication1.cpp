// QtApplication1.cpp
#include "QtApplication1.h"
#include<QCloseEvent>
#include<iostream>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

QtApplication1::QtApplication1(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::QtApplication1Class)
{
    ui->setupUi(this);
    graphview = new GraphViewClass(this); // Instantiate the new window
    connect(ui->getstartedButton, &QPushButton::clicked, this, &QtApplication1::on_getstartedButton_clicked);
}

QtApplication1::~QtApplication1()
{
    delete ui;
}


void QtApplication1::on_getstartedButton_clicked()
{
    graphview->showMaximized();
    this->hide();
}

