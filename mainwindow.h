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

    void aslan_suka_tyt_obnovlyaesh();
    void a_tyt_blyat_renderish(QPainter &painter);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
