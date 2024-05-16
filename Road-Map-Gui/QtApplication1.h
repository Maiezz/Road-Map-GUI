// QtApplication1.h
#pragma once

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>
#include "ui_QtApplication1.h"
#include "GraphView.h" // Include the header file for the new window
#include "CountryGraph.h"
class QtApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtApplication1(QWidget* parent = nullptr);
    ~QtApplication1();
    

public slots:
    void on_signupButton_clicked();
    void on_loginButton_clicked();
    void on_loginButton_2_clicked();
    void on_getstartedButton_clicked(); // Slot to handle button click

private:
    Ui::QtApplication1Class* ui;
    GraphViewClass* graphview;  // Declare an instance of the new window
    QStackedWidget* stackedWidget;
    UserManager manager;
    UserGraph *newuser;
    UserGraph* UserLoggedin;

protected:
    void closeEvent(QCloseEvent* event) override;
};


