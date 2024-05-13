#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "ui_MainWindow.h"

class Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    Mainwindow(QWidget* parent = nullptr);
    ~Mainwindow();

private slots:
    void on_exitButton2_clicked();
    void on_Add_city_clicked();
    void on_Add_road_clicked();
    void on_Delete_city_clicked();
    void on_Delete_road_clicked();
    void on_sumbit();

private:
    Ui::MainwindowClass* ui;

};