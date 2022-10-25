#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::aslan_suka_tyt_obnovlyaesh);
    timer->start(10);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    a_tyt_blyat_renderish(painter);
}



void MainWindow::aslan_suka_tyt_obnovlyaesh() {
    this->engine.update();
    this->update();
}

void MainWindow::a_tyt_blyat_renderish(QPainter &painter) {
    this->engine.render(painter);
}

