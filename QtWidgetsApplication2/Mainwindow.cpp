#include "Mainwindow.h"
#include <QMainWindow>
#include "CountryGraph.h"
int flag = 0;



Mainwindow::Mainwindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainwindowClass)
{
    ui->setupUi(this);
    // Hide certain UI elements initially
    ui->city_name2->setVisible(false);
    ui->city_name1->setVisible(false);
    ui->road_cost->setVisible(false);
    ui->home->setVisible(false);

    // Connect signals to slots
    connect(ui->Add_road, &QPushButton::clicked, this, &Mainwindow::on_Add_road_clicked);
    connect(ui->Delete_city, &QPushButton::clicked, this, &Mainwindow::on_Delete_city_clicked);
    connect(ui->Delete_road, &QPushButton::clicked, this, &Mainwindow::on_Delete_road_clicked);
    connect(ui->Submit, &QPushButton::clicked, this, &Mainwindow::on_sumbit);  // Assuming there's a button named submit in your UI.
}

Mainwindow::~Mainwindow()
{
    delete ui;
}

void Mainwindow::on_exitButton2_clicked()
{
    close();


}

void Mainwindow::on_Add_city_clicked()
{
    flag = 0;
    ui->city_name2->setVisible(true);
    ui->city_name1->setVisible(false);
    ui->road_cost->setVisible(false);
    ui->home->setVisible(true);
}

void Mainwindow::on_Add_road_clicked()
{
    flag = 1;
    ui->city_name2->setVisible(true);
    ui->city_name1->setVisible(true);
    ui->road_cost->setVisible(true);
}

void Mainwindow::on_Delete_city_clicked()
{
    flag = 3;
    ui->city_name2->setVisible(true);
    ui->city_name1->setVisible(false);
    ui->road_cost->setVisible(false);
}

void Mainwindow::on_Delete_road_clicked()
{
    flag = 4;
    ui->city_name2->setVisible(true);
    ui->city_name1->setVisible(true);
    ui->road_cost->setVisible(false);
}

void Mainwindow::on_sumbit()
{
    switch (flag) {
    case 0:
    {
        QString city = ui->city_name2->text();

    }
    }
}