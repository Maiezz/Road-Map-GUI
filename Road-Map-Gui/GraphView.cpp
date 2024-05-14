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
    connect(ui->toalgo, &QPushButton::clicked, this, &GraphViewClass::toalgowindow);
    connect(ui->tograph, &QPushButton::clicked, this, &GraphViewClass::tographwindow);
    connect(ui->redobutton, &QPushButton::clicked, this, &GraphViewClass::Redo);
    connect(ui->undobutton, &QPushButton::clicked, this, &GraphViewClass::Undo);
    connect(ui->prims, &QPushButton::clicked, this, &GraphViewClass::showline);
    connect(ui->deletecitybutton, &QPushButton::clicked, this, &GraphViewClass::DeleteCity);
    connect(ui->deleteedgebutton, &QPushButton::clicked, this, &GraphViewClass::DeleteEdge);
    stackedWidget = findChild<QStackedWidget*>("stackedWidget");
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
    QString cityName = ui->city_name1->text();
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

void GraphViewClass::toalgowindow()
{
    stackedWidget->setCurrentIndex(1);
}

void GraphViewClass::tographwindow()
{
    stackedWidget->setCurrentIndex(0);
}

void GraphViewClass::showline()
{
    queue<string> q;
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");
    q.push("sss");

    // Get the widget inside the scroll area
    QWidget* widgetInsideScrollArea = ui->scrollArea->findChild<QWidget*>("scrollAreaWidgetContents");

    // Create a vertical layout to hold labels
    QVBoxLayout* layout = new QVBoxLayout(widgetInsideScrollArea);

    // Loop through the queue and create labels
    while (!q.empty()) {
        QLabel* label = new QLabel(QString::fromStdString(q.front()), widgetInsideScrollArea);
        layout->addWidget(label);
        q.pop();
    }

    // Set the layout of the widget inside the scroll area
    widgetInsideScrollArea->setLayout(layout);

}


void GraphViewClass::addCity()
{
    addingCity = true;
    ui->addCityButton->setText("Click on the map to add the city");
    ui->addCityButton->setEnabled(false);
    ui->city_name1->setEnabled(false);

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
                    return true; // Event handled
                }
                QPointF pos = view->mapToScene(mouseEvent->pos());
                cityPositions.insert(cityName, pos);
                Country.AddCity(cityName.toStdString());

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

