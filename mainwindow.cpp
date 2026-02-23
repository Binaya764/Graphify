#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsTextItem>
#include <QTimer>
#include <cmath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    graph(6),
    currentStep(0)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);   // graphicsView from UI
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
    timer = new QTimer(this);
    timer->setInterval(1500);

    // After ui->setupUi(this);
    ui->tableView->setColumnCount(3); // Step | Visited Node | Shortest Distance
    QStringList headers;
    headers << "Node" << "Distance" << "Visited";
    ui->tableView->setHorizontalHeaderLabels(headers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->scrollToBottom();
    connect(timer, &QTimer::timeout,
            this, &MainWindow::next_step);
    connect(ui->BtnStart,
            &QPushButton::clicked,
            this,
            &MainWindow::startDijkstra);

    // Predefined graph
    graph.addEdge(0, 1, 4);
    graph.addEdge(0, 2, 2);
    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 5);
    graph.addEdge(2, 3, 8);
    graph.addEdge(2, 4, 10);
    graph.addEdge(3, 4, 2);
    graph.addEdge(3, 5, 6);
    graph.addEdge(4, 5, 3);

    drawGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::drawGraph()
{
    int n = graph.size();
    int radius = 150;
    int centerX = 250;
    int centerY = 250;

    const auto& adj = graph.getAdjList();

    std::vector<QPointF> positions;

    //node
    for(int i = 0; i < n; i++) {
        double angle = 2 * M_PI * i / n;
        int x = centerX + radius * cos(angle);
        int y = centerY + radius * sin(angle);

        positions.push_back(QPointF(x, y));

        QGraphicsEllipseItem* node =
            scene->addEllipse(x, y, 40, 40,
                              QPen(Qt::black),
                              QBrush(QColor(0x00, 0xE5, 0xFF)));

        node->setZValue(0);   // circle behind text
        nodeItems.push_back(node);

        QGraphicsTextItem* label =
            scene->addText(QString::number(i));

        label->setFont(QFont("Arial", 12, QFont::Bold));
        label->setDefaultTextColor(Qt::black);
        label->setPos(x + 12, y + 8);
        label->setZValue(1);   // text above circle
    }
    //edge
    for(int u = 0; u < n; u++) {
        for(const Edge& e : adj[u]) {
            if(u < e.to) {
                scene->addLine(QLineF(positions[u]+QPointF(20,20),
                                      positions[e.to]+QPointF(20,20)));

                QPointF mid = (positions[u] + positions[e.to]) / 2;
                scene->addText(QString::number(e.weight))->setPos(mid);
            }
        }
    }
}

void MainWindow::startDijkstra()
{
    currentStep = 0;

    int n = graph.size();

    ui->tableView->setRowCount(n);

    for(int i = 0; i < n; i++)
    {
        ui->tableView->setItem(i, 0,
                               new QTableWidgetItem(QString::number(i)));

        ui->tableView->setItem(i, 1,
                               new QTableWidgetItem("∞"));

        ui->tableView->setItem(i, 2,
                               new QTableWidgetItem("No"));
    }

    steps = Dijkstra::run(graph, 0);

    timer->start();
}
void MainWindow::next_step()
{
    if(currentStep >= steps.size())
    {
        timer->stop();
        return;
    }

    Step s = steps[currentStep];
    int u = s.currentNode;

    const int INF = std::numeric_limits<int>::max();

    // -------- GRAPH UPDATE --------

    // Reset all nodes to default color
    for(auto node : nodeItems)
        node->setBrush(QBrush(QColor(0x00, 0xE5, 0xFF)));  // original blue

    // Color previously visited nodes light gray
    for(int i = 0; i < currentStep; i++)
    {
        int visitedNode = steps[i].currentNode;
        nodeItems[visitedNode]->setBrush(QBrush(Qt::lightGray));
    }

    // Highlight current node yellow
    nodeItems[u]->setBrush(QBrush(Qt::yellow));


    // -------- TABLE UPDATE --------

    for(int i = 0; i < s.dist.size(); i++)
    {
        QTableWidgetItem* distItem = ui->tableView->item(i, 1);
        if(!distItem) continue;

        if(s.dist[i] == INF)
            distItem->setText("∞");
        else
            distItem->setText(QString::number(s.dist[i]));
    }

    QTableWidgetItem* visitedItem = ui->tableView->item(u, 2);
    if(visitedItem)
        visitedItem->setText("Yes");

    // Highlight table row
    for(int i = 0; i < ui->tableView->rowCount(); i++)
    {
        for(int col = 0; col < 3; col++)
        {
            QTableWidgetItem* item = ui->tableView->item(i, col);
            if(!item) continue;

            // Default background and text
            item->setBackground(Qt::white);
            item->setForeground(Qt::black);
        }
    }

    // Highlight current node row in yellow
    for(int col = 0; col < 3; col++)
    {
        QTableWidgetItem* item = ui->tableView->item(u, col);
        if(!item) continue;

        item->setBackground(QColor(255, 255, 150));
        item->setForeground(Qt::black);  // ensure text is visible
    }

    // Optional: Gray out visited nodes
    for(int i = 0; i < currentStep; i++)
    {
        int visitedNode = steps[i].currentNode;
        if(visitedNode == u) continue; // skip current

        for(int col = 0; col < 3; col++)
        {
            QTableWidgetItem* item = ui->tableView->item(visitedNode, col);
            if(!item) continue;

            item->setBackground(Qt::lightGray);
            item->setForeground(Qt::black);
        }
    }

    currentStep++;
}
