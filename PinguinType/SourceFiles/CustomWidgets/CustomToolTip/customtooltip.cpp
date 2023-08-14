
#include "customtooltip.h"
#include <QPainter>
#include <QPainterPath>
#include <QStyle>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

CustomToolTip::CustomToolTip(QWidget *parent):
    QFrame(parent)
{
    move_timer.setInterval(time/10);
    connect(&move_timer, SIGNAL(timeout()), this, SLOT(anim_move()));
}

void CustomToolTip::paintEvent(QPaintEvent *e) {
    QPainter *p = new QPainter(this);

    QFont f("Comic Sans MS", 15);
    QFontMetrics fm(f);
    int width = fm.horizontalAdvance(str);

    QPainterPath tooltip_path, text_path;
    tooltip_path.setFillRule(Qt::FillRule::WindingFill);
    text_path.setFillRule(Qt::FillRule::WindingFill);

    if (arrow_pos == ArrowPos::top) {
        tooltip_path.addRoundedRect(0,5,width + 40,35,5, 5);

        tooltip_path.moveTo((width+10)/2.0 + 18.0,0);
        tooltip_path.lineTo((width+10)/2.0+15.0 + 18.0, 15);
        tooltip_path.lineTo((width+10)/2.0-15.0 + 18.0, 15);
        tooltip_path.lineTo((width+10)/2.0 + 18.0,0);

        text_path.addText(18, 29, f, str);

        this->resize(width+40, 40);
    } else if (arrow_pos == ArrowPos::right) {
        tooltip_path.addRoundedRect(0,0,width + 40,35,5,5);

        tooltip_path.moveTo(width + 45 + 2, 17.5);
        tooltip_path.lineTo(width + 45 + 2 - 15.0, 17.5 + 15);
        tooltip_path.lineTo(width + 45 + 2 - 15.0, 17.5 - 15);
        tooltip_path.lineTo(width + 45 + 2, 17.5);

        text_path.addText(18, 24, f, str);

        this->resize(width+40 + 5 + 2, 35);
    } else if (arrow_pos == ArrowPos::bottom) {
        tooltip_path.addRoundedRect(0,0,width + 40, 35, 5, 5);

        tooltip_path.moveTo((width+10)/2.0 + 18.0,41);
        tooltip_path.lineTo((width+10)/2.0+5.0 + 18.0, 35);
        tooltip_path.lineTo((width+10)/2.0-5.0 + 18.0, 35);
        tooltip_path.lineTo((width+10)/2.0 + 18.0,41);

        text_path.addText(18, 24, f, str);

        this->resize(width+40, 41);
    } else if (arrow_pos == ArrowPos::left) {
        tooltip_path.addRoundedRect(7,0,width + 40,35,5,5);

        tooltip_path.moveTo(0, 17.5);
        tooltip_path.lineTo(0 + 7, 17.5 + 6);
        tooltip_path.lineTo(0 + 7, 17.5 - 6);
        tooltip_path.lineTo(0, 17.5);

        text_path.addText(18, 24, f, str);

        this->resize(width+40 + 5 + 2, 35);
    } else if (arrow_pos == ArrowPos::top_mid_right) {
        tooltip_path.addRoundedRect(0,5,width + 40,35,5, 5);

        tooltip_path.moveTo((width+10)/2.0 + 18.0 + (width+10)/4.0,0);
        tooltip_path.lineTo((width+10)/2.0+15.0 + 18.0 + (width+10)/4.0, 15);
        tooltip_path.lineTo((width+10)/2.0-15.0 + 18.0 + (width+10)/4.0, 15);
        tooltip_path.lineTo((width+10)/2.0 + 18.0 + (width+10)/4.0,0);

        text_path.addText(18, 29, f, str);

        this->resize(width+40, 40);
    } else if (arrow_pos == ArrowPos::top_right) {
        tooltip_path.addRoundedRect(0,5,width + 40,35,5, 5);

        tooltip_path.moveTo((width+10) + 18.0,0);
        tooltip_path.lineTo((width+10)+4.0 + 18.0, 5);
        tooltip_path.lineTo((width+10)-4.0 + 18.0, 5);
        tooltip_path.lineTo((width+10) + 18.0, 0);

        text_path.addText(18, 29, f, str);

        this->resize(width+40, 41);
    }

    p->setRenderHint(QPainter::Antialiasing);
    p->fillPath(tooltip_path, QColor(0x101011));
    p->fillPath(text_path, QColor(0xF1F1F1));
    p->end();
}

void CustomToolTip::set_text(QString str) {
    this->str = str;
}

void CustomToolTip::set_arrow_pos(ArrowPos arrow_pos) {
    this->arrow_pos = arrow_pos;
}

void CustomToolTip::set_pos(QPoint cur_pos) {
    this->cur_pos = cur_pos;
}

void CustomToolTip::custom_show() {

    QGraphicsOpacityEffect *fade_effect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animation = new QPropertyAnimation(fade_effect, "opacity");
    animation->setEasingCurve(QEasingCurve::Type::InCurve);
    animation->setDuration(time);
    this->setGraphicsEffect(fade_effect);
    animation->setStartValue(0.01);
    animation->setEndValue(1.0);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
    is_show_move = true;

    if (arrow_pos == ArrowPos::top) {
        tmp_pos.setY(cur_pos.y() - 10);
        tmp_pos.setX(cur_pos.x());
    } else if (arrow_pos == ArrowPos::right) {
        tmp_pos.setY(cur_pos.y());
        tmp_pos.setX(cur_pos.x() + 10);
    } else if (arrow_pos == ArrowPos::bottom) {
        tmp_pos.setY(cur_pos.y() + 10);
        tmp_pos.setX(cur_pos.x());
    } else if (arrow_pos == ArrowPos::left) {
        tmp_pos.setY(cur_pos.y());
        tmp_pos.setX(cur_pos.x() - 10);
    } else if (arrow_pos == ArrowPos::top_mid_right) {
        tmp_pos.setY(cur_pos.y() - 10);
        tmp_pos.setX(cur_pos.x());
    } else if (arrow_pos == ArrowPos::top_right) {
        tmp_pos.setY(cur_pos.y() - 10);
        tmp_pos.setX(cur_pos.x());
    }

    this->show();

    move_timer.start();
}

void CustomToolTip::custom_hide() {
    QGraphicsOpacityEffect *fade_effect = new QGraphicsOpacityEffect(this);
    this->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animation = new QPropertyAnimation(fade_effect, "opacity");
    animation->setEasingCurve(QEasingCurve::Type::InCurve);
    animation->setDuration(time);
    this->setGraphicsEffect(fade_effect);
    animation->setStartValue(1.0);
    animation->setEndValue(0);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
    is_show_move = false;

    tmp_pos.setY(cur_pos.y());
    tmp_pos.setX(cur_pos.x());

    move_timer.start();
}

void CustomToolTip::anim_move() {
    if ( is_show_move && cur_pos == tmp_pos) {
        move_timer.stop();
        return;
    }

    if (!is_show_move && (cur_pos.x() - 10 == tmp_pos.x() ||
        cur_pos.x() + 10 == tmp_pos.x() ||
        cur_pos.y() - 10 == tmp_pos.y() ||
        cur_pos.y() + 10 == tmp_pos.y())) {
        move_timer.stop();
        this->hide();
        return;
    }

    this->move(tmp_pos);

    if (arrow_pos == ArrowPos::top) {
        if (is_show_move)
            tmp_pos.setY(tmp_pos.y() + 1);
        else
            tmp_pos.setY(tmp_pos.y() - 1);
    } else if (arrow_pos == ArrowPos::right) {
        if (is_show_move)
            tmp_pos.setX(tmp_pos.x() - 1);
        else
            tmp_pos.setX(tmp_pos.x() + 1);
    } else if (arrow_pos == ArrowPos::bottom) {
        if (is_show_move)
            tmp_pos.setY(tmp_pos.y() - 1);
        else
            tmp_pos.setY(tmp_pos.y() + 1);
    }
    else if (arrow_pos == ArrowPos::left) {
        if (is_show_move)
            tmp_pos.setX(tmp_pos.x() + 1);
        else
            tmp_pos.setX(tmp_pos.x() - 1);
    } else if (arrow_pos == ArrowPos::top_mid_right) {
        if (is_show_move)
            tmp_pos.setY(tmp_pos.y() + 1);
        else
            tmp_pos.setY(tmp_pos.y() - 1);
    } else if (arrow_pos == ArrowPos::top_right) {
        if (is_show_move)
            tmp_pos.setY(tmp_pos.y() + 1);
        else
            tmp_pos.setY(tmp_pos.y() - 1);
    }
}

void CustomToolTip::update_size() {
    QFont f("Comic Sans MS", 15);
    QFontMetrics fm(f);
    int width = fm.horizontalAdvance(str);

    if (arrow_pos == ArrowPos::top) {
        this->resize(width+40, 40);
    } else if (arrow_pos == ArrowPos::right) {
        this->resize(width+40 + 5 + 2, 35);
    } else if (arrow_pos == ArrowPos::bottom) {
        this->resize(width+40, 41);
    } else if (arrow_pos == ArrowPos::left) {
        this->resize(width+40 + 5 + 2, 35);
    } else if (arrow_pos == ArrowPos::top_mid_right) {
        this->resize(width+40, 40);
    } else if (arrow_pos == ArrowPos::top_right) {
        this->resize(width+40, 40);
    }
}
