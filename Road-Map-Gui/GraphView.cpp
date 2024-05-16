#include "GraphView.h"
#include <unordered_map>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <QInputDialog>
#include <QPixmap>
#include<QThread>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <QVBoxLayout>
#include <String>
string  stringformat(string str) {
    string result = "";
    result += toupper(str[0]);
    for (int i = 1; i < str.size(); i++) {
        result += tolower(str[i]);
    }
    return result;
}
void ReadFormFiles(CountryGraph& Country)
{
    Country.Read_Cities_FromFiles("cities.txt");
    Country.Read_Edges_FromFiles("edges.txt");
}
void WriteInFiles(CountryGraph& Country)
{
    Country.Write_Cities_InFiles("cities.txt");
    Country.Write_Edges_InFiles("edges.txt");
}

GraphViewClass::GraphViewClass(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::GraphViewClassClass)
{


    Country.applychanges = false;
    ReadFormFiles(Country);
    Country.applychanges = true;
    cities = Country.getCities();

    ui->setupUi(this);
    flag = 0;
    // Set up the QGraphicsView from the UI
    view = ui->graphicsView;
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    readCityPositionsFromFile();
    drawGraph();




    connect(ui->addCityButton, &QPushButton::clicked, this, &GraphViewClass::addCity);
    connect(ui->addEdgeButton, &QPushButton::clicked, this, &GraphViewClass::addEdge);
    connect(ui->toalgo, &QPushButton::clicked, this, &GraphViewClass::toalgowindow);
    connect(ui->tograph, &QPushButton::clicked, this, &GraphViewClass::tographwindow);
    connect(ui->redobutton, &QPushButton::clicked, this, &GraphViewClass::Redo);
    connect(ui->undobutton, &QPushButton::clicked, this, &GraphViewClass::Undo);
    connect(ui->deletecitybutton, &QPushButton::clicked, this, &GraphViewClass::DeleteCity);
    connect(ui->deleteedgebutton, &QPushButton::clicked, this, &GraphViewClass::DeleteEdge);
    connect(ui->Submit, &QPushButton::clicked, this, &GraphViewClass::sub);
    stackedWidget = findChild<QStackedWidget*>("stackedWidget");
    // Connect the QGraphicsView mouse press event to MainWindow
    connect(ui->prims, &QPushButton::clicked, this, &GraphViewClass::showprims);
    connect(ui->dijkistra, &QPushButton::clicked, this, &GraphViewClass::showdijkistra);
    connect(ui->kruksal, &QPushButton::clicked, this, &GraphViewClass::showkruksal);
    connect(ui->dfs, &QPushButton::clicked, this, &GraphViewClass::showdfs);
    connect(ui->bfs, &QPushButton::clicked, this, &GraphViewClass::showbfs);
    connect(ui->floyd, &QPushButton::clicked, this, &GraphViewClass::showfloyd);

    stackedWidget->setCurrentIndex(0);
    view->installEventFilter(this);
}

GraphViewClass::~GraphViewClass()
{
    saveCityPositionsToFile();
    WriteInFiles(Country);
    delete ui;
}

void GraphViewClass::addCity()
{

    flag = 0;
    ui->city_name2->setVisible(false);
    ui->city_name1->setVisible(true);
    ui->road_cost->setVisible(false);
    

}
void GraphViewClass::addEdge()
{
    flag = 1;
    ui->city_name2->setVisible(true);
    ui->city_name1->setVisible(true);
    ui->road_cost->setVisible(true);

}
void GraphViewClass::DeleteCity()
{
    flag = 2;
    ui->city_name1->setVisible(true);
    ui->city_name2->setVisible(false);
    ui->road_cost->setVisible(false);
}

void GraphViewClass::DeleteEdge()
{
    flag = 3;
    ui->city_name2->setVisible(true);
    ui->city_name1->setVisible(true);
    ui->road_cost->setVisible(false);
}
void GraphViewClass::sub()
{
    switch (flag) {
    case 0://add city
    {
        addingCity = true;
        ui->addCityButton->setText("Click on the map to add the city");
        ui->city_name1->setEnabled(false);
        break;
    }

    case 1://add edge 
    {
        QString source = ui->city_name1->text();

        if (source.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a city name.");
            return; // Event handled
        }

        QString dest = ui->city_name2->text();

        if (dest.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a city name.");
            return; // Event handled
        }

        QString costtext = ui->road_cost->text();
        int cost = costtext.toInt();
        if (dest.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a city cost.");
            return; // Event handled
        }
        while (cost < 0)
        {
            QMessageBox::warning(this, "Error", "cost must be nonnegative number");
            ui->road_cost->clear();
            QString costtext = ui->road_cost->text();
            int cost = costtext.toInt();
        }

        Country.AddEdge(stringformat(source.toStdString()), stringformat(dest.toStdString()), cost);
        drawGraph();
        ui->city_name1->clear();
        ui->city_name2->clear();
        ui->road_cost->clear();
        break;
    }

    case 2:// delete city
    {
        QString cityName = ui->city_name1->text();
        if (Country.FindCity(stringformat(cityName.toStdString()))) {
            if (cityName.isEmpty()) {
                QMessageBox::warning(this, "Error", "Please enter a city name.");
                return;
            }
            Country.DeleteCity(stringformat(cityName.toStdString()));
            drawGraph();
           
        }
        else 
            QMessageBox::warning(this, "Error", "You can not delete unexisted city ");
        ui->city_name1->clear();
        break;
    }
    case 3:// delete edge
    {
        QString source = ui->city_name1->text();

        if (source.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a city name.");
            return; // Event handled
        }

        QString dest = ui->city_name2->text();

        if (dest.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a city name.");
            return; // Event handled
        }
        if (Country.FindEdge(stringformat(source.toStdString()), stringformat(dest.toStdString()))) {
            Country.DeleteEdge(stringformat(source.toStdString()), stringformat(dest.toStdString()));
            drawGraph();
        }
        else 
            QMessageBox::warning(this, "Error", "You can not delete unexisted edge ");
        ui->city_name1->clear();
        ui->city_name2->clear();
        break;
    }
    }
}



///////////////////////////////////////////////////
void GraphViewClass::Redo()
{
    Country.Redo();
    drawGraph();

}

void GraphViewClass::Undo()
{
    Country.Undo();
    drawGraph();

}

void GraphViewClass::toalgowindow()
{
    stackedWidget->setCurrentIndex(1);
}

void GraphViewClass::tographwindow()
{
    stackedWidget->setCurrentIndex(0);
}

bool GraphViewClass::eventFilter(QObject* watched, QEvent* event)
{

    if (watched == view && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            if (addingCity) {
                // Add city at the clicked position
                QString cityName = ui->city_name1->text();
               
                if (cityName.isEmpty()) {

                    QMessageBox::warning(this, "Error", "Please enter a city name.");
                    addingCity = false;
                    ui->city_name1->clear();
                    ui->addCityButton->setText("Add City");
                    ui->addCityButton->setEnabled(true);
                    ui->city_name1->setEnabled(true);
                    return true; // Event handled
                }
                QPointF pos = view->mapToScene(mouseEvent->pos());
                cityPositions.insert(QString::fromStdString(stringformat(cityName.toStdString())), pos);
                Country.AddCity(stringformat(cityName.toStdString()));

                drawGraph();

                addingCity = false;
                ui->city_name1->clear();
                ui->addCityButton->setText("Add City");
                ui->addCityButton->setEnabled(true);
                ui->city_name1->setEnabled(true);
                return true; // Event handled
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}


void GraphViewClass::saveCityPositionsToFile()
{
    // Open a file for writing
    QString fileName = "city_positions.txt";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return;
    }

    // Create a text stream for writing to the file
    QTextStream out(&file);

    // Iterate over each city and its position in the QMap
    for (auto it = cityPositions.constBegin(); it != cityPositions.constEnd(); ++it) {
        QString cityName = it.key();
        QPointF position = it.value();
        out << cityName << "," << position.x() << "," << position.y() << "\n";
    }

    // Close the file
    file.close();
}

void GraphViewClass::readCityPositionsFromFile()
{
    // Open the file for reading
    QString fileName = "city_positions.txt";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading:" << file.errorString();
        return;
    }

    // Create a text stream for reading from the file
    QTextStream in(&file);

    // Read the data line by line and populate the cityPositions QMap
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() == 3) {
            QString cityName = parts[0];
            qreal x = parts[1].toDouble();
            qreal y = parts[2].toDouble();
            cityPositions.insert(cityName, QPointF(x, y));
        }
    }

    // Close the file
    file.close();
}

void GraphViewClass::drawGraph()
{
    scene->clear();
    vector<pair<string, edge>> edges = Country.getEdges();

    for (const auto& edgePair : edges) {
        const string& source = edgePair.first;
        const edge& edgeInfo = edgePair.second;

        // Retrieve the positions of the source and destination cities
        QPointF startPos = cityPositions.value(QString::fromStdString(source));
        QPointF endPos = cityPositions.value(QString::fromStdString(edgeInfo.destination_city));

        if (!startPos.isNull() && !endPos.isNull()) {
            // Draw the edge between the source and destination cities
            QPen pen(Qt::gray); // Set edge color to gray
            pen.setWidth(5); // Set edge width to 5 (adjust as needed)
            QGraphicsLineItem* edge = scene->addLine(startPos.x(), startPos.y(), endPos.x(), endPos.y(), pen);
            edge->setToolTip(QString("%1 -> %2: %3").arg(QString::fromStdString(source)).arg(QString::fromStdString(edgeInfo.destination_city)).arg(edgeInfo.cost));
        }
    }

    // Load the pin image
    QPixmap pinImage("img/pin2.png");
    unordered_map<string, list<edge>> cities = Country.getCities();
    // Iterate over each city and its position in the QMap
    for (auto city : cities) {
        QString cityName = QString::fromStdString(city.first);
        QPointF position = cityPositions.value(QString::fromStdString(city.first));

        // Add a QGraphicsPixmapItem as a pin at the position
        QGraphicsPixmapItem* pinItem = scene->addPixmap(pinImage);
        pinItem->setOffset(-pinImage.width() / 2, -pinImage.height()); // Center the pin on the position
        pinItem->setPos(position);
        pinItem->setToolTip(cityName);

        // Add text item for city name
        QGraphicsTextItem* textItem = scene->addText(cityName);
        textItem->setDefaultTextColor(Qt::black);
        textItem->setPos(position.x() - pinImage.width() / 2, position.y() + 5); // Adjust y position to place under pin
    }

}

void GraphViewClass::showprims()
{
    if (!Country.is_graphempty()) {
        if (Country.is_connected()) {
            queue <pair<string, edge>> msp = Country.Prims();
            // Get the widget inside the scroll area
            QWidget* widgetInsideScrollArea = ui->scrollArea->findChild<QWidget*>("scrollAreaWidgetContents");
            clearLayout(widgetInsideScrollArea);
            // Create a vertical layout to hold labels
            QVBoxLayout* layout = new QVBoxLayout(widgetInsideScrollArea);

            // Loop through the queue and create labels
            while (!msp.empty()) {
                QString source = QString::fromStdString(msp.front().first);
                QString dist = QString::fromStdString(msp.front().second.destination_city);
                QString cost = QString::number(msp.front().second.cost);
                QLabel* label = new QLabel("From : " + source + "To : " + dist + "By : " + cost, widgetInsideScrollArea);
                layout->addWidget(label);
                msp.pop();
            }

            // Set the layout of the widget inside the scroll area
            widgetInsideScrollArea->setLayout(layout);
        }
        else
            QMessageBox::warning(this, "Error", "You can not apply prims on disconnected graph,use kruksal instead of it");
    }
    else {
        QMessageBox::warning(this, "Error", "You can not apply prims on empty graph ");
    }
}

void GraphViewClass::showbfs()
{
    string start = "Cairo";
    if (Country.FindCity(start)) {
        queue<string> bfs = Country.BFS(start);
        // Get the widget inside the scroll area
        QWidget* widgetInsideScrollArea = ui->scrollArea->findChild<QWidget*>("scrollAreaWidgetContents");
        clearLayout(widgetInsideScrollArea);
        // Create a vertical layout to hold labels
        QVBoxLayout* layout = new QVBoxLayout(widgetInsideScrollArea);

        // Loop through the queue and create labels
        while (!bfs.empty()) {
            QString source = QString::fromStdString(bfs.front());
            QLabel* label = new QLabel(source, widgetInsideScrollArea);
            layout->addWidget(label);
            bfs.pop();
        }

        // Set the layout of the widget inside the scroll area
        widgetInsideScrollArea->setLayout(layout);
    }
    else
        QMessageBox::warning(this, "Error", "Start city not in the graph");
}

void GraphViewClass::showdfs()
{
    string start = "Cairo";
    if (Country.FindCity(start)) {
        queue<string> dfs = Country.DFS(start);
        // Get the widget inside the scroll area
        QWidget* widgetInsideScrollArea = ui->scrollArea->findChild<QWidget*>("scrollAreaWidgetContents");
        clearLayout(widgetInsideScrollArea);
        // Create a vertical layout to hold labels
        QVBoxLayout* layout = new QVBoxLayout(widgetInsideScrollArea);

        // Loop through the queue and create labels
        while (!dfs.empty()) {
            QString source = QString::fromStdString(dfs.front());
            QLabel* label = new QLabel(source, widgetInsideScrollArea);
            layout->addWidget(label);
            dfs.pop();
        }

        // Set the layout of the widget inside the scroll area
        widgetInsideScrollArea->setLayout(layout);
    }
    else
        QMessageBox::warning(this, "Error", "Start city not in the graph");
}

void GraphViewClass::showkruksal()
{
    if (!Country.is_graphempty()) {
        queue <pair<string, edge>> msp = Country.kruskalMST();
        // Get the widget inside the scroll area
        QWidget* widgetInsideScrollArea = ui->scrollArea->findChild<QWidget*>("scrollAreaWidgetContents");
        clearLayout(widgetInsideScrollArea);
        // Create a vertical layout to hold labels
        QVBoxLayout* layout = new QVBoxLayout(widgetInsideScrollArea);

        // Loop through the queue and create labels
        while (!msp.empty()) {
            QString source = QString::fromStdString(msp.front().first);
            QString dist = QString::fromStdString(msp.front().second.destination_city);
            QString cost = QString::number(msp.front().second.cost);
            QLabel* label = new QLabel("From : " + source + "To : " + dist + "By : " + cost, widgetInsideScrollArea);
            layout->addWidget(label);
            msp.pop();
        }

        // Set the layout of the widget inside the scroll area
        widgetInsideScrollArea->setLayout(layout);
    }else
        QMessageBox::warning(this, "Error", "You can not apply kruksal on empty graph ");
}

void GraphViewClass::showfloyd()
{   
    string city1 = "Cairo";
    string city2 = "Giza";
    if (Country.FindCity(city1) and Country.FindCity(city2)) {
        pair<int, vector<string>> path = Country.FloydWarshall2(city1, city2);
        vector<string> path_steps = path.second;

        // Get the widget inside the scroll area
        QWidget* widgetInsideScrollArea = ui->scrollArea->findChild<QWidget*>("scrollAreaWidgetContents");
        clearLayout(widgetInsideScrollArea);
        // Create a vertical layout to hold labels
        QVBoxLayout* layout = new QVBoxLayout(widgetInsideScrollArea);

        // Loop through the path steps and create labels
        for (const string& step : path_steps) {
            QString step_label = QString::fromStdString(step);
            QLabel* label = new QLabel(step_label, widgetInsideScrollArea);
            layout->addWidget(label);
        }

        // Set the layout of the widget inside the scroll area
        widgetInsideScrollArea->setLayout(layout);
    }
    else 
        QMessageBox::warning(this, "Error", "City one or city two are not founed");
}

void GraphViewClass::showdijkistra()
{
    if (!Country.is_graphempty()) {
        string start = "Cairo";
        queue<string> dij = Country.dijkstra_algorithm(start);
        // Get the widget inside the scroll area
        QWidget* widgetInsideScrollArea = ui->scrollArea->findChild<QWidget*>("scrollAreaWidgetContents");
        clearLayout(widgetInsideScrollArea);
        // Create a vertical layout to hold labels
        QVBoxLayout* layout = new QVBoxLayout(widgetInsideScrollArea);

        // Loop through the queue and create labels
        while (!dij.empty()) {
            QString source = QString::fromStdString(dij.front());
            QLabel* label = new QLabel(source, widgetInsideScrollArea);
            layout->addWidget(label);
            dij.pop();
        }

        // Set the layout of the widget inside the scroll area
        widgetInsideScrollArea->setLayout(layout);
    }
    else
        QMessageBox::warning(this, "Error", "The graph is empty");
}

void GraphViewClass::clearLayout(QWidget* widget)
{
    QLayout* layout = widget->layout();
    if (layout)
    {
        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }
        delete layout;
    }
}