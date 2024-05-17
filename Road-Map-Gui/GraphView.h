#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <ui_GraphView.h>
#include <CountryGraph.h>

QT_BEGIN_NAMESPACE
namespace Ui { class GraphViewClass; }
QT_END_NAMESPACE

class GraphViewClass : public QMainWindow
{
    Q_OBJECT

public:
    GraphViewClass(QWidget* parent = nullptr);
    ~GraphViewClass();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void addCity();
    void addEdge();
    void Redo();
    void Undo();
    void DeleteCity();
    void DeleteEdge();
    void toalgowindow();
    void tographwindow();
    void sub();
    void DeleteAll();
    ////////////////
    void showprims();
    void showbfs();
    void showdfs();
    void showkruksal();

    void showdijkistra();
    /////////////////////
private:
    int flag;
    void saveCityPositionsToFile();
    void drawGraph();
    void readCityPositionsFromFile();
    Ui::GraphViewClassClass* ui;
    QGraphicsView* view;
    QGraphicsScene* scene;
    QPushButton* addCityButton;
    QPushButton* addEdgeButton;
    QLineEdit* cityNameLineEdit;
    bool addingCity = false;
    bool addingEdge = false;
    QPointF edgeStartPoint;
    QString sourceNode;
    QString destNode;
    int edgeCost;
    QMap<QString, QPointF> cityPositions;

    // Reference to the CountryGraph object
    CountryGraph Country;

    // Data structure to hold cities and edges
    unordered_map<string, list<edge>> cities;
    QStackedWidget* stackedWidget;
    void clearLayout(QWidget* widget);
};
