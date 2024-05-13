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
void ReadFormFiles(CountryGraph& Country)
{
    Country.Read_Cities_FromFiles();
    Country.Read_Edges_FromFiles();
}
void WriteInFiles(CountryGraph& Country)
{
    Country.Write_Cities_InFiles();
    Country.Write_Edges_InFiles();
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

    // Set up the QGraphicsView from the UI
    view = ui->graphicsView;
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    readCityPositionsFromFile();
    drawGraph();




    connect(ui->addCityButton, &QPushButton::clicked, this, &GraphViewClass::addCity);
    connect(ui->addEdgeButton, &QPushButton::clicked, this, &GraphViewClass::addEdge);
    connect(ui->mspbutton, &QPushButton::clicked, this, &GraphViewClass::Displaymsp);
    connect(ui->dfsbutton, &QPushButton::clicked, this, &GraphViewClass::traverse);
    connect(ui->bfsbutton, &QPushButton::clicked, this, &GraphViewClass::traverse);
    connect(ui->returntograph, &QPushButton::clicked, this, &GraphViewClass::drawGraph);
    connect(ui->redobutton, &QPushButton::clicked, this, &GraphViewClass::Redo);
    connect(ui->undobutton, &QPushButton::clicked, this, &GraphViewClass::Undo);
    connect(ui->deletecitybutton, &QPushButton::clicked, this, &GraphViewClass::DeleteCity);
    connect(ui->deleteedgebutton, &QPushButton::clicked, this, &GraphViewClass::DeleteEdge);
    // Connect the QGraphicsView mouse press event to MainWindow
    view->installEventFilter(this);
}

GraphViewClass::~GraphViewClass()
{
    saveCityPositionsToFile();
    WriteInFiles(Country);
    delete ui;
}


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

void GraphViewClass::DeleteCity()
{
    QString cityName = ui->cityNameLineEdit->text();
    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a city name.");
        return;
    }
    Country.DeleteCity(cityName.toStdString());
    drawGraph();

}

void GraphViewClass::DeleteEdge()
{

}

void GraphViewClass::traverse()
{
    QString cityName = ui->startCitybutton->text();
    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a city name.");
        return; // Event handled
    }
    queue<string> path;
    // Check which button triggered the slot
    QObject* sender = QObject::sender();
    if (sender == ui->bfsbutton) {
        path = Country.BFS(cityName.toStdString());
    }
    else if (sender == ui->dfsbutton) {
        path = Country.DFS(cityName.toStdString());
    }


    // Create custom color for DFS edges
    QColor dfsEdgeColor(Qt::red);

    // Set pen style for DFS edges to dashed
    Qt::PenStyle penStyle = Qt::DashLine;

    // Draw the DFS path on the scene
    while (!path.empty()) {
        string currentCity = path.front();
        path.pop();
        if (!path.empty()) {
            string nextCity = path.front();

            // Retrieve positions of current and next cities from cityPositions
            QPointF currentPos = cityPositions.value(QString::fromStdString(currentCity));
            QPointF nextPos = cityPositions.value(QString::fromStdString(nextCity));

            // If both current and next positions are found, draw the DFS edge between them
            if (!currentPos.isNull() && !nextPos.isNull()) {
                QGraphicsLineItem* dfsEdgeItem = new QGraphicsLineItem(currentPos.x(), currentPos.y(), nextPos.x(), nextPos.y());
                QPen dfsEdgePen(dfsEdgeColor);
                dfsEdgePen.setStyle(penStyle); // Set pen style to dashed
                dfsEdgePen.setWidth(2); // Adjust width as needed
                dfsEdgeItem->setPen(dfsEdgePen);
                scene->addItem(dfsEdgeItem);

            }

        }
    }
}

void GraphViewClass::Displaymsp()
{
    // Get the minimum spanning tree (MSP) using Prim's algorithm
    queue<pair<string, edge>> msp = Country.Prims();

    // Create custom color for MSP edges
    QColor mspEdgeColor(Qt::black);

    // Set pen style for MSP edges to dashed
    Qt::PenStyle penStyle = Qt::DashLine;

    // Draw the edges of the MSP on the scene
    while (!msp.empty()) {
        // Get the next edge from the queue
        auto [sourceCity, e] = msp.front();
        msp.pop();

        // Retrieve positions of source and destination cities from cityPositions
        QPointF sourcePos = cityPositions.value(QString::fromStdString(sourceCity));
        QPointF destPos = cityPositions.value(QString::fromStdString(e.destination_city));

        // If both source and destination positions are found, draw the MSP edge between them
        if (!sourcePos.isNull() && !destPos.isNull()) {
            QGraphicsLineItem* mspEdgeItem = new QGraphicsLineItem(sourcePos.x(), sourcePos.y(), destPos.x(), destPos.y());
            QPen mspEdgePen(mspEdgeColor);
            mspEdgePen.setStyle(penStyle); // Set pen style to dashed
            mspEdgePen.setWidth(2); // Adjust width as needed
            mspEdgeItem->setPen(mspEdgePen);
            scene->addItem(mspEdgeItem);
        }
    }
}
void GraphViewClass::addCity()
{
    addingCity = true;
    ui->addCityButton->setText("Click on the map to add the city");
    ui->addCityButton->setEnabled(false);
    ui->cityNameLineEdit->setEnabled(false);

}



bool GraphViewClass::eventFilter(QObject* watched, QEvent* event)
{

    if (watched == view && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            if (addingCity) {
                // Add city at the clicked position
                QString cityName = ui->cityNameLineEdit->text();
                if (cityName.isEmpty()) {
                    QMessageBox::warning(this, "Error", "Please enter a city name.");
                    return true; // Event handled
                }
                QPointF pos = view->mapToScene(mouseEvent->pos());
                cityPositions.insert(cityName, pos);
                Country.AddCity(cityName.toStdString());

                drawGraph();

                addingCity = false;
                ui->cityNameLineEdit->clear();
                ui->addCityButton->setText("Add City");
                ui->addCityButton->setEnabled(true);
                ui->cityNameLineEdit->setEnabled(true);
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


void GraphViewClass::addEdge()
{
    bool ok;
    QString source = QInputDialog::getText(this, "Enter Source Node", "Source Node:", QLineEdit::Normal, "", &ok);
    if (!ok || source.isEmpty()) return;

    QString dest = QInputDialog::getText(this, "Enter Source Node", "Destination Node:", QLineEdit::Normal, "", &ok);
    if (!ok || dest.isEmpty()) return;

    int cost = QInputDialog::getInt(this, "Enter Edge Cost", "Edge Cost:", 0, 0, INT_MAX, 1, &ok);
    if (!ok) return;

    Country.AddEdge(source.toStdString(), dest.toStdString(), cost);
    drawGraph();
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

