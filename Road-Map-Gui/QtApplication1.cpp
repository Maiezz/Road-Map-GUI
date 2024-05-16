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
    connect(ui->signupButton, &QPushButton::clicked, this, &QtApplication1::on_signupButton_clicked);
    connect(ui->loginButton, &QPushButton::clicked, this, &QtApplication1::on_loginButton_clicked);
    connect(ui->getstartedButton, &QPushButton::clicked, this, &QtApplication1::on_getstartedButton_clicked);
    connect(ui->loginButton_2, &QPushButton::clicked, this, &QtApplication1::on_loginButton_2_clicked);
    stackedWidget = findChild<QStackedWidget*>("stackedWidget");
    stackedWidget->setVisible(false);
    manager.loadUsers();
    manager.loadAllGraphs();
}

QtApplication1::~QtApplication1()
{
    manager.saveUsres();
    delete ui;
}

void QtApplication1::on_loginButton_clicked()
{
    stackedWidget->setVisible(true);
    stackedWidget->setCurrentIndex(1);
}

void QtApplication1::on_loginButton_2_clicked()
{
    string username = (ui->loginusername->text()).toStdString();
    string password = (ui->loginpassword->text()).toStdString();
    bool found = false;
    for (auto& user : manager.users)
    {
        if (user.username == username && user.password == password)
        {
            UserLoggedin = &user;
           found = true;
           // if user logged in
           graphview->showMaximized();
           this->hide();

        }
    }
    if (!found) {
        QMessageBox::warning(this, "Error", "You Dont Have an Account Sign in first");
        ui->loginusername->clear();
        ui->loginpassword->clear();
        stackedWidget->setCurrentIndex(0);
    }
   
}

void QtApplication1::on_getstartedButton_clicked()
{
    stackedWidget->setVisible(true);
    stackedWidget->setCurrentIndex(0);
}

void QtApplication1::on_signupButton_clicked()
{
    string username = (ui->signinusername->text()).toStdString();
    string password = (ui->signinpassword->text()).toStdString();
    
    for (auto& user : manager.users)
    {
        if (user.username == username && user.password == password)
        {
            QMessageBox::warning(this, "Error", "You Already Have an Account");
            ui->signinusername->clear();
            ui->loginpassword->clear();
            return;
        }
    }

    newuser = new UserGraph(username, password);
    newuser->createFiles();
    manager.users.push_back(*newuser);
    QMessageBox::warning(this, "Signed Up Successfully", "Login to your Account");
    stackedWidget->setCurrentIndex(1);
}

void QtApplication1::closeEvent(QCloseEvent* event)
{


    event->accept(); // Allow the window to close
}