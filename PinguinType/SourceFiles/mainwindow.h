
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMouseEvent>
#include <QDateTime>
#include <QTime>
#include <QTimer>

#include "LoginInterface/logininterface.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_hide_window_btn_clicked();
    void on_close_window_btn_clicked();
    void change_time();

private:
    Ui::MainWindow *ui;
    QPoint cur_point;
    bool pressed = false;
    int h, m, s;

};

#endif // MAINWINDOW_H
