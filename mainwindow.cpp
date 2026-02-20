#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsTextItem>
#include <QtMath>
QColor softBlue(111, 168, 220); // R=111, G=168, B=220
QBrush brush(softBlue);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
    ui->graphicsView->setRenderHint(QPainter::TextAntialiasing, true);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform, true);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    ui->graphicsView->setScene(scene);

    setupGraph();
    generatePositions();
    drawGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BtnStart_clicked()
{
    qDebug() << "Button clicked";

}

void MainWindow::setupGraph()
{
    graph.resize(4);

    graph[0].push_back({1, 5});
    graph[0].push_back({2, 3});

    graph[1].push_back({2, 2});
    graph[1].push_back({3, 6});

    graph[2].push_back({3, 7});
}

void MainWindow::generatePositions()
{
    int n = graph.size();
    int centerX = 250;
    int centerY = 250;
    int radius = 150;

    for (int i = 0; i < n; i++) {
        double angle = 2 * M_PI * i / n;

        double x = centerX + radius * cos(angle);
        double y = centerY + radius * sin(angle);

        positions.push_back(QPointF(x, y));
    }
}

void MainWindow::drawGraph()
{


    // Draw Edges
    for (int i = 0; i < graph.size(); i++) {
        for (Edge e : graph[i]) {

            QPointF p1 = positions[i];
            QPointF p2 = positions[e.to];

            scene->addLine(p1.x() + 20,
                           p1.y() + 20,
                           p2.x() + 20,
                           p2.y() + 20,
                           QPen(Qt::black));

            QGraphicsTextItem *weightText =
                scene->addText(QString::number(e.weight));

            weightText->setDefaultTextColor(Qt::black);
            QFont font("Segoe UI", 12);
            weightText->setFont(font);


            weightText->setPos((p1.x() -3+ p2.x()) / 2,
                               (p1.y() + p2.y()) / 2);

        }
    }

    // Draw Nodes
    for (int i = 0; i < graph.size(); i++) {

        QGraphicsEllipseItem *node =
            scene->addEllipse(positions[i].x(),
                              positions[i].y(),
                              40, 40,
                              QPen(Qt::black),
                              QBrush(softBlue));

        nodes.push_back(node);

        QGraphicsTextItem *text =
            scene->addText(QString::number(i));

        text->setPos(positions[i].x() + 15,
                     positions[i].y() + 10);
    }
}
