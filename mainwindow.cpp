#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsTextItem>
#include <QTimer>
#include <cmath>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    graph(6),
    currentStep(0)
{
    ui->setupUi(this);   // 🔥 THIS loads mainwindow.ui

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);   // graphicsView from UI

    timer = new QTimer(this);
    timer->setInterval(1000);

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
                              QBrush(Qt::white));

        nodeItems.push_back(node);
        scene->addText(QString::number(i))->setPos(x+12, y+8);
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
    steps = Dijkstra::run(graph, 0); // Source = 0
    currentStep = 0;
    timer->start();
}
void MainWindow::next_step()
{
    if(currentStep >= steps.size()) {
        timer->stop();
        return;
    }

    // Reset all nodes to white
    for(auto node : nodeItems)
        node->setBrush(QBrush(Qt::white));

    int u = steps[currentStep].currentNode;

    // Highlight current node
    nodeItems[u]->setBrush(QBrush(Qt::yellow));

    currentStep++;
}

