#include "customdialog.h"
#include "ui_customdialog.h"
#include <QApplication>
#include <QMouseEvent>
#include <QFrame>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QJsonArray>

CustomDialog::CustomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomDialog)
{
    ui->setupUi(this);
    this->parent = dynamic_cast<MainWindow*>(parent);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_NoSystemBackground, true);

    QPixmap loupe_pixmap = QPixmap(":images/icons/icons/loupe.png");
    this->ui->loupe_lbl->setPixmap(QPixmap(":images/icons/icons/loupe.png").scaled(22, 22));

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    QVBoxLayout *layout = new QVBoxLayout(this->ui->scrollAreaWidgetContents);
    layout->setContentsMargins(2,0,0,0);

    QFile file;
    file.setFileName(QDir::currentPath() + "/themes.json");

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    const QJsonObject &object = d.object();

    foreach (const QString &key, object.keys()) {
        qDebug() << key;

        if (key == "current" || key == "current_name")
            continue;

        theme_vec.push_back(new customframe());

        if (key == d.object()["current_name"])
            theme_vec.back()->select();

        theme_vec.back()->set_name(key);
        theme_vec.back()->set_theme(object);



        connect(theme_vec.back(), SIGNAL(change_theme(QString)), parent, SLOT(theme(QString)));
        connect(theme_vec.back(), SIGNAL(tmp_change_theme(QString)), parent, SLOT(tmp_theme(QString)));

        layout->addWidget(theme_vec.back());
    }

    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignTop);

    /*theme_vec.reserve(50);

    for (int i = 0; i < 50; i++) {
        theme_vec[i] = new customframe();
        theme_vec[i]->set_name("creamsicle");
        theme_vec[i]->set_theme(object);
        layout->addWidget(theme_vec[i]);
    }*/
}

CustomDialog::~CustomDialog()
{
    qDebug() << this->children();

    for (auto x:theme_vec)
        delete x;

    delete ui;
}

void CustomDialog::on_theme_find_textChanged(const QString &arg1)
{
    if (this->ui->theme_find->text().size() == 0) {
        for (auto x:theme_vec)
            x->show();
    } else {
        QString find_str = this->ui->theme_find->text();

        for (auto x:theme_vec) {
            if (!x->get_name().startsWith(find_str)) {
                x->hide();
            } else {
                x->show();
            }
        }

    }
}

void CustomDialog::custom_hide() {
    this->ui->theme_find->clear();

    for (auto x:theme_vec)
        x->show();

    this->close();
}

void CustomDialog::set_theme(const QJsonObject &object) {
    QJsonObject obj = object["current"].toObject();

    QString qss("QFrame#main_frame{border-radius:10px;"
                "background-color: #"+obj["main_bg_color"].toString()+
                ";border: 4px solid #"+obj["main_border_color"].toString()+";}");
    this->ui->main_frame->setStyleSheet(qss);
    qss.clear();
    qss.append("font: 18px \"Comic Sans MS\";color: #"+obj["line_edit_text_color"].toString()+";border: 0px black solid;");
    this->ui->theme_find->setStyleSheet(qss);

    bool *ok = new bool();
    QPixmap source_image(":images/icons/icons/loupe.png");
    QPainter painter(&source_image);
    QRgb base_color = obj["custom_frame_font_color"].toString().toInt(ok, 16);

    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//::CompositionMode_SourceIn
    painter.fillRect(source_image.rect(), base_color);
    painter.end();
    this->ui->loupe_lbl->setPixmap(source_image.scaled(22, 22));

    qss.clear();
    qss.append("QScrollArea#scrollArea{background-color:transparent;border-radius:20px;}"
                "QScrollBar:vertical{background-color: #"+obj["main_bg_color"].toString()+";"
                "width: 6px;margin: 0px 0px 0px 0px;border: 0px solid black;border-radius: 4px;}"
                "QScrollBar::handle:vertical{background-color: #"+obj["line_edit_text_color"].toString()+";"
                "min-height: 5px;border-radius: 2px;}QScrollBar::sub-line:vertical{margin: 3px 0px 3px 0px;"
                "height: 10px;width: 10px;subcontrol-position: top;subcontrol-origin: margin;"
                "background-color:transparent;}QScrollBar::add-line:vertical{margin: 3px 0px 3px 0px;"
                "background-color:transparent;height: 10px;width: 10px;subcontrol-position: bottom;"
                "subcontrol-origin: margin;}QScrollBar::sub-line:vertical:hover,QScrollBar::sub-line:vertical:on"
                "{background-color:transparent;height: 10px;width: 10px;subcontrol-position: top;"
                "subcontrol-origin: margin;}QScrollBar::add-line:vertical:hover, QScrollBar::add-line:vertical:on"
                "{background-color:transparent;height: 10px;width: 10px;subcontrol-position: bottom;"
                "subcontrol-origin: margin;}QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical"
                "{background-color: transparent;}QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical"
                                                          "{background-color:transparent;}");

    this->ui->scrollArea->setStyleSheet(qss);

    for (auto x:theme_vec) {
        x->unselect();

        if (x->get_name() == object["current_name"].toString())
            x->select();

        x->set_theme(object);
    }
}

