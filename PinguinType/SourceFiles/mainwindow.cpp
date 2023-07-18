
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->logo_btn->setCursor(Qt::PointingHandCursor);
    this->ui->move_frame->installEventFilter(this);
    QTime tmp =  QTime::currentTime();
    this->ui->time_label->setText(QString::number(tmp.hour()) + ":" +
                                  QString::number(tmp.minute()) + ":" +
                                  QString::number(tmp.second()));

    QTimer *main_timer = new QTimer();
    connect(main_timer, SIGNAL(timeout()), this, SLOT(change_time()));
    main_timer->start(1000);

    /*QPixmap source_image(":/images/icons/icons/example.png");

    QRgb base_color = 0x5F4F7F;
    QPixmap new_image = source_image;
    QPainter painter(&new_image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(new_image.rect(), base_color);
    painter.end();
    QImage tmp_im;
    tmp_im = new_image.toImage();
    tmp_im.save("../Resources/icons/example.png");*/

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_hide_window_btn_clicked()
{
    this->showMinimized();
}


void MainWindow::on_close_window_btn_clicked()
{
    QApplication::quit();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    cur_point = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    if(pressed)
        this->move(mapToParent(event->pos() - cur_point));
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->move_frame && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = (QMouseEvent*)event;
        if (pressed == false){
            cur_point = mouseEvent->pos();
        }
        pressed = true;
        return true;
    }
    if (object == ui->move_frame && event->type() == QEvent::MouseButtonRelease)
    {
        pressed = false;
        return true;
    }
    else
        return false;
}

void MainWindow::change_time() {
    QTime tmp =  QTime::currentTime();
    QString s = QString::number(tmp.second());
    QString m = QString::number(tmp.minute());
    QString h = QString::number(tmp.hour());

    if (s.size() == 1)
        s.push_front('0');

    if (m.size() == 1)
        m.push_front('0');

    if (h.size() == 1)
        h.push_front('0');

    this->ui->time_label->setText(h + ":" + m + ":" + s);
}


void MainWindow::on_main_login_btn_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_start_test_btn_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(1);
}

