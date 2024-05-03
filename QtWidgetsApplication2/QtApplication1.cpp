// QtApplication1.cpp
#include "QtApplication1.h"

QtApplication1::QtApplication1(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::QtApplication1Class)
{
    ui->setupUi(this);
    mainwindow = new Mainwindow(this); // Instantiate the new window
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
    mainwindow->showFullScreen(); // Show the new window 
    this->hide(); // Hide the current window
}

void QtApplication1::on_getstartedButton_clicked()
{
    stackedWidget->setVisible(true);
    stackedWidget->setCurrentIndex(0);
}

void QtApplication1::on_signupButton_clicked()
{
    stackedWidget->setCurrentIndex(1);
}