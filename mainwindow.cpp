#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::MainWindow_update);
    timer->setInterval(1000/60);
    timer->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    MainWindow_render(painter);
}



void MainWindow::MainWindow_update() {
    this->engine.update();
    this->update();
}

void MainWindow::MainWindow_render(QPainter &painter) {
    this->engine.render(painter);
}
