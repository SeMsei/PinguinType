
#include "customframe.h"
#include <QEvent>
#include <QPainter>
#include <QCursor>

customframe::customframe(QWidget *parent):
    QFrame(parent)
{

    right_main_frame = new QFrame(this);
    right_main_layout = new QHBoxLayout();
    right_main_frame->setLayout(right_main_layout);

    main_layout = new QHBoxLayout();
    left_frame = new QFrame(this);
    right_frame = new QFrame(this);
    right_frame->setMaximumHeight(20);
    right_frame->setMinimumWidth(56);
    left_layout = new QHBoxLayout();
    right_layout = new QHBoxLayout();
    left_frame->setLayout(left_layout);
    right_frame->setLayout(right_layout);
    check_mark_lbl = new QLabel(this);
    theme_name_lbl = new QLabel(this);
    this->theme_name_lbl->setText("123123");
    this->check_mark_lbl->setMinimumSize(QSize(16, 16));
    this->check_mark_lbl->setMaximumSize(QSize(16, 16));

    left_layout->addWidget(check_mark_lbl);
    left_layout->addWidget(theme_name_lbl);
    left_layout->setAlignment(Qt::AlignLeft);
    left_layout->setSpacing(0);
    left_layout->setContentsMargins(5, 0, 5, 0);

    first_clr = new QLabel(this);
    second_clr = new QLabel(this);
    third_clr = new QLabel(this);

    first_clr->setMinimumSize(12, 12);
    second_clr->setMinimumSize(12, 12);
    third_clr->setMinimumSize(12, 12);
    first_clr->setMaximumSize(12, 12);
    second_clr->setMaximumSize(12, 12);
    third_clr->setMaximumSize(12, 12);

    right_layout->addWidget(first_clr);
    right_layout->addWidget(second_clr);
    right_layout->addWidget(third_clr);
    right_layout->setSpacing(6);
    right_layout->setContentsMargins(5, 0, 5, 0);

    right_main_frame->setLayout(right_main_layout);
    right_main_layout->addWidget(right_frame);
    right_main_layout->setAlignment(Qt::AlignRight);
    right_main_layout->setSpacing(0);
    right_main_layout->setContentsMargins(5, 0, 5, 0);

    main_layout->addWidget(left_frame);
    main_layout->addWidget(right_main_frame);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(5, 0, 5, 0);



    this->setLayout(main_layout);

    this->installEventFilter(this);

    this->setMaximumHeight(30);
    this->setMinimumHeight(30);

    emit_timer.setSingleShot(true);
    emit_timer.setInterval(300);
    connect(&emit_timer, SIGNAL(timeout()), this, SLOT(emit_tmp_change_theme()));

    this->setCursor(Qt::PointingHandCursor);
}

customframe::~customframe() {
    //clear memory

    //qDebug() << 123 << this->children();

    delete first_clr;
    delete second_clr;
    delete third_clr;
    delete right_layout;
    delete right_frame;
    delete right_main_layout;
    delete right_main_frame;

    delete check_mark_lbl;
    delete theme_name_lbl;
    delete left_layout;
    delete left_frame;

    delete main_layout;

    //qDebug() << this->children();

}

void customframe::set_name(QString name) {
    this->name = name;

    theme_name_lbl->setText(name);
}

QString customframe::get_name() {
    return name;
}

void customframe::select() {
    is_slctd = true;
}

void customframe::set_theme(const QJsonObject& obj) {
    bool *ok = new bool();

    std_bg_color = obj["current"]["custom_frame_bg_color"].toString().toInt(ok, 16);
    font_color = obj["current"]["custom_frame_font_color"].toString().toInt(ok, 16);
    hvr_bg_color = obj["current"]["custom_frame_hvr_bg_color"].toString().toInt(ok, 16);
    first_bg_color = obj[name]["logo_icon_color"].toString().toInt(ok, 16);
    second_bg_color = obj[name]["btn_transparent_std_pic_font_color"].toString().toInt(ok, 16);
    third_bg_color = obj[name]["logo_font_color"].toString().toInt(ok, 16);
    right_bg_color = obj[name]["main_bg_color"].toString().toInt(ok, 16);


    QString qss;
    //qss.append("background-color:#"+QString::number(std_bg_color, 16)+";")
    this->right_frame->setStyleSheet("border-radius: 10px;background-color: #"+QString::number(right_bg_color, 16)+";");
    this->first_clr->setStyleSheet("border-radius: 6px;background-color: #"+QString::number(first_bg_color, 16)+";");
    //qDebug() << QString::number(first_bg_color, 16) << this->first_clr->styleSheet() ;
    this->second_clr->setStyleSheet("border-radius: 6px;background-color: #"+QString::number(second_bg_color, 16)+";");
    this->third_clr->setStyleSheet("border-radius: 6px;background-color: #"+QString::number(third_bg_color, 16)+";");

    if (!is_hovered) {
        this->theme_name_lbl->setStyleSheet("font: 14px \"Comic Sans MS\";color: #"+QString::number(font_color, 16)+";");
        this->setStyleSheet("background-color: #"+QString::number(std_bg_color, 16)+";");
    } else {
        this->theme_name_lbl->setStyleSheet("font: 14px \"Comic Sans MS\";color: #"+QString::number(std_bg_color, 16)+";");
        this->setStyleSheet("background-color: #"+QString::number(hvr_bg_color, 16)+";");
    }

    //qDebug() << this->styleSheet();

    if (is_slctd) {
        QPixmap source_image(":images/icons/icons/example.png");
        QPainter painter(&source_image);
        QRgb base_color = font_color;

        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//::CompositionMode_SourceIn
        painter.fillRect(source_image.rect(), base_color);
        painter.end();

        this->check_mark_lbl->setPixmap(source_image.scaled(16, 16));
    } else {
        this->check_mark_lbl->setPixmap(QPixmap().scaled(16, 16));
    }

    delete ok;

    //font_color
}

bool customframe::eventFilter(QObject *obj, QEvent *e) {
    if (e->type() == QEvent::Enter && obj == this) {
        hover();
    }

    if (e->type() == QEvent::Leave && obj == this) {
        leave();
    }

    return false;
}

void customframe::hover() {
    this->setStyleSheet("background-color: #"+QString::number(hvr_bg_color, 16)+";");
    this->theme_name_lbl->setStyleSheet("font: 14px \"Comic Sans MS\";color: #"+QString::number(std_bg_color, 16)+";");
    is_hovered = true;

    if (is_slctd) {
        QPixmap source_image(":images/icons/icons/example.png");
        QPainter painter(&source_image);
        QRgb base_color = std_bg_color;

        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//::CompositionMode_SourceIn
        painter.fillRect(source_image.rect(), base_color);
        painter.end();

        this->check_mark_lbl->setPixmap(source_image.scaled(16, 16));
    }

    emit_timer.start();
}

void customframe::leave() {
    this->setStyleSheet("background-color: #"+QString::number(std_bg_color, 16)+";");
    this->theme_name_lbl->setStyleSheet("font: 14px \"Comic Sans MS\";color: #"+QString::number(font_color, 16)+";");
    is_hovered = false;

    if (is_slctd) {
        QPixmap source_image(":images/icons/icons/example.png");
        QPainter painter(&source_image);
        QRgb base_color = font_color;

        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//::CompositionMode_SourceIn
        painter.fillRect(source_image.rect(), base_color);
        painter.end();

        this->check_mark_lbl->setPixmap(source_image.scaled(16, 16));
    }

    emit_timer.stop();
}

void customframe::mouseReleaseEvent(QMouseEvent *e) {
    emit change_theme(this->name);
}

void customframe::emit_tmp_change_theme() {
    emit tmp_change_theme(this->name);
}

void customframe::unselect() {
    this->is_slctd = false;
}
