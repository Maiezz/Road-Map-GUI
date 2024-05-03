#pragma once
#include <QMainWindow>
#include "ui_Mainwindow.h"
class Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    Mainwindow(QWidget* parent = nullptr);
    ~Mainwindow();

private:
    Ui::MainwindowClass* ui;
 
};