#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include "graph.h"
#include "dijkstra.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void drawGraph();
    void startDijkstra();
    void next_step();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    Graph graph;
    std::vector<Step> steps;
    std::vector<QGraphicsEllipseItem*> nodeItems;
    int currentStep;
    QTimer *timer;
};

#endif
