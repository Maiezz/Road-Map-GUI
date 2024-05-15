// QtApplication1.cpp
#include "QtApplication1.h"
#include<QCloseEvent>
#include<iostream>
#include <QFile>
#include <QTextStream>

QtApplication1::QtApplication1(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::QtApplication1Class)
{
    ui->setupUi(this);
    graphview = new GraphViewClass(this); // Instantiate the new window
    connect(ui->signupButton, &QPushButton::clicked, this, &QtApplication1::on_signupButton_clicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &QtApplication1::on_loginButton_clicked);
    connect(ui->getstartedButton, &QPushButton::clicked, this, &QtApplication1::on_getstartedButton_clicked);
    connect(ui->loginButton_2, &QPushButton::clicked, this, &QtApplication1::on_loginButton_2_clicked);
    stackedWidget = findChild<QStackedWidget*>("stackedWidget");
    stackedWidget->setVisible(false);
}

QtApplication1::~QtApplication1()
{
    delete ui;
}

void QtApplication1::on_loginButton_clicked()
{
    stackedWidget->setVisible(true);
    stackedWidget->setCurrentIndex(1);
}

void QtApplication1::on_loginButton_2_clicked()
{
    QString username = ui->loginusername->text();
    QString password = ui->loginpassword->text();



    // if user logged in
    graphview->showMaximized(); // Show the new window 
    this->hide(); // Hide the current window
}

void QtApplication1::on_getstartedButton_clicked()
{
    stackedWidget->setVisible(true);
    stackedWidget->setCurrentIndex(0);
}

void QtApplication1::on_signupButton_clicked()
{
    QString username = ui->signinusername->text();
    QString password = ui->signinpassword->text();


    // if user signed in
    stackedWidget->setCurrentIndex(1);
}

void QtApplication1::closeEvent(QCloseEvent* event)
{


    event->accept(); // Allow the window to close
}