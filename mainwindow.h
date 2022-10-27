#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "engine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Engine engine;

    void paintEvent(QPaintEvent *);

    void MainWindow_update();
    void MainWindow_render(QPainter &painter);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
