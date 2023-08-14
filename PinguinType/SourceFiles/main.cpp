
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlag(Qt::FramelessWindowHint);
    //w.setWindowState(Qt::WindowFullScreen);
    w.setAttribute(Qt::WA_NoSystemBackground, true);
    w.setAttribute(Qt::WA_TranslucentBackground, true);
    w.setMinimumSize(QSize(QGuiApplication::primaryScreen()->size().width()*0.8,
                           QGuiApplication::primaryScreen()->size().height()*0.8));
    w.setMaximumSize(QSize(QGuiApplication::primaryScreen()->size().width()*0.8,
                           QGuiApplication::primaryScreen()->size().height()*0.8));
    w.show();
    w.set_tooltip();
    return a.exec();
}
