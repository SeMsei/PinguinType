#include "animatedhoverbutton.h"

#include <QEvent>
#include <QDebug>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionButton>

AnimatedHoverButton::AnimatedHoverButton(QWidget *parent) :
    QPushButton(parent),
    pic_font_clr_anim(this, "color")
{
    this->installEventFilter(this);
    update_qss();
}

void AnimatedHoverButton::set_pic_font_color(const QColor& color)
{
    m_color = color;

    if (!is_bg_transparent) {
        m_bg_color.setRed(2.0*mid_r - color.red());
        m_bg_color.setGreen(2.0*mid_g - color.green());
        m_bg_color.setBlue(2.0*mid_b - color.blue());
    }

    this->update_qss();

}

const QColor& AnimatedHoverButton::get_pic_font_color() const {
    return m_color;
}

bool AnimatedHoverButton::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::HoverEnter) {
        StartHoverEnterAnimation();
        is_enter = true;
        emit hover_signal();
    }

    if (e->type() == QEvent::HoverLeave) {
        StartHoverLeaveAnimation();
        is_enter = false;
        emit leave_signal();
    }

    return false;
}

void AnimatedHoverButton::StartHoverEnterAnimation()
{
    pic_font_clr_anim.stop();
    pic_font_clr_anim.setDuration(100);
    pic_font_clr_anim.setStartValue(QColor(this->std_pic_font_clr));
    pic_font_clr_anim.setEndValue(QColor(this->hvr_pic_font_clr));
    pic_font_clr_anim.setEasingCurve(QEasingCurve::Linear);

    pic_font_clr_anim.start();
}

void AnimatedHoverButton::StartHoverLeaveAnimation()
{
    pic_font_clr_anim.stop();
    pic_font_clr_anim.setDuration(100);
    pic_font_clr_anim.setStartValue(QColor(this->hvr_pic_font_clr));
    pic_font_clr_anim.setEndValue(QColor(this->std_pic_font_clr));
    pic_font_clr_anim.setEasingCurve(QEasingCurve::Linear);
    pic_font_clr_anim.start();
}

void AnimatedHoverButton::set_bg_transparent(bool is_bg_transparent) {
    this->is_bg_transparent = is_bg_transparent;
}

void AnimatedHoverButton::set_std_pic_font_clr(QRgb std_pic_font_clr) {
    this->std_pic_font_clr = std_pic_font_clr;
    this->hvr_bg_clr = std_pic_font_clr;
    m_color = std_pic_font_clr;
    QColor tmp_std(std_pic_font_clr);
    QColor tmp_hvr(hvr_pic_font_clr);

    mid_r = std::min(tmp_std.red(), tmp_hvr.red()) + abs(tmp_std.red() - tmp_hvr.red()) / 2.0;
    mid_g = std::min(tmp_std.green(), tmp_hvr.green()) + abs(tmp_std.green() - tmp_hvr.green()) / 2.0;
    mid_b = std::min(tmp_std.blue(), tmp_hvr.blue()) + abs(tmp_std.blue() - tmp_hvr.blue()) / 2.0;
}

void AnimatedHoverButton::set_std_bg_clr(QRgb std_bg_clr) {
    this->std_bg_clr = std_bg_clr;
    QColor tmp_std(std_pic_font_clr);
    QColor tmp_hvr(hvr_pic_font_clr);

    mid_r = std::min(tmp_std.red(), tmp_hvr.red()) + abs(tmp_std.red() + tmp_hvr.red()) / 2.0;
    mid_g = std::min(tmp_std.green(), tmp_hvr.green()) + abs(tmp_std.green() + tmp_hvr.green()) / 2.0;
    mid_b = std::min(tmp_std.blue(), tmp_hvr.blue()) + abs(tmp_std.blue() + tmp_hvr.blue()) / 2.0;
}

void AnimatedHoverButton::set_hvr_pic_font_clr(QRgb hvr_pic_font_clr) {
    this->hvr_pic_font_clr = hvr_pic_font_clr;
    this->std_bg_clr = hvr_pic_font_clr;
    m_bg_color = hvr_pic_font_clr;
    QColor tmp_std(std_pic_font_clr);
    QColor tmp_hvr(hvr_pic_font_clr);

    mid_r = std::min(tmp_std.red(), tmp_hvr.red()) + abs(tmp_std.red() - tmp_hvr.red()) / 2.0;
    mid_g = std::min(tmp_std.green(), tmp_hvr.green()) + abs(tmp_std.green() - tmp_hvr.green()) / 2.0;
    mid_b = std::min(tmp_std.blue(), tmp_hvr.blue()) + abs(tmp_std.blue() - tmp_hvr.blue()) / 2.0;
}

void AnimatedHoverButton::set_hvr_bg_clr(QRgb hvr_bg_clr) {
    this->hvr_bg_clr = hvr_bg_clr;
    QColor tmp_std(std_pic_font_clr);
    QColor tmp_hvr(hvr_pic_font_clr);

    mid_r = std::min(tmp_std.red(), tmp_hvr.red()) + abs(tmp_std.red() - tmp_hvr.red()) / 2.0;
    mid_g = std::min(tmp_std.green(), tmp_hvr.green()) + abs(tmp_std.green() - tmp_hvr.green()) / 2.0;
    mid_b = std::min(tmp_std.blue(), tmp_hvr.blue()) + abs(tmp_std.blue() - tmp_hvr.blue()) / 2.0;
}

void AnimatedHoverButton::set_pic(QString pic_path) {
    this->pic_path = pic_path;
    source_image = QPixmap(pic_path);
}

void AnimatedHoverButton::update_qss() {
    QString qss;

    if (is_clicked)
        qss.append("border:2px solid #"+QString::number(border_color, 16)+"; border-radius: 10px;font: "+QString::number(font_size)+"px \"Comic Sans MS\";");
    else
        qss.append("border:0px solid transparent; border-radius: 10px;font: "+QString::number(font_size)+"px \"Comic Sans MS\";");

    QString color_str = QString("color:rgb(%1, %2, %3);").arg(m_color.red()).arg(m_color.green()).arg(m_color.blue());
    qss.append(color_str);

    if (!is_bg_transparent) {
        QString color_str = QString("background-color: rgb(%1, %2, %3);").arg(m_bg_color.red()).arg(m_bg_color.green()).arg(m_bg_color.blue());
        qss.append(color_str);
    } else {
        qss.append("background-color:transparent;");
    }

    this->setStyleSheet(qss);

    if (pic_path.size()) {
        QPainter painter(&source_image);
        QRgb base_color = m_color.rgb();

        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//::CompositionMode_SourceIn
        painter.fillRect(source_image.rect(), base_color);
        painter.end();
        btn_icon.addPixmap(source_image);
        this->setIcon(btn_icon);
    }
}

void AnimatedHoverButton::set_font_size(int font_size) {
    this->font_size = font_size;
}

void AnimatedHoverButton::set_theme(const QJsonObject &obj) {
    bool *ok = new bool();

    if (is_bg_transparent) {
        std_pic_font_clr = obj["btn_transparent_std_pic_font_color"].toString().toInt(ok, 16);
        std_bg_clr = obj["btn_transparent_hvr_pic_font_color"].toString().toInt(ok, 16);
        hvr_pic_font_clr = obj["btn_transparent_hvr_pic_font_color"].toString().toInt(ok, 16);
        hvr_bg_clr = obj["btn_transparent_std_pic_font_color"].toString().toInt(ok, 16);
        sltctd_hvr_pic_font_color = obj["btn_transparent_sltctd_hvr_pic_font_color"].toString().toInt(ok, 16);
        sltctd_std_pic_font_color = obj["btn_transparent_sltctd_std_pic_font_color"].toString().toInt(ok, 16);

        if (!is_clicked) {
            m_color = std_pic_font_clr;
            m_bg_color = std_bg_clr;
        } else {
            m_color = hvr_pic_font_clr;
            m_bg_color = hvr_bg_clr;
        }
    } else {
        std_pic_font_clr = obj["btn_non_transparent_std_pic_font_color"].toString().toInt(ok, 16);
        m_color = std_pic_font_clr;


        std_bg_clr = obj["btn_non_transparent_hvr_pic_font_color"].toString().toInt(ok, 16);
        m_bg_color = std_bg_clr;
        qDebug() << m_color.red() << m_color.green() << m_color.blue() << std_bg_clr;
        hvr_pic_font_clr = obj["btn_non_transparent_hvr_pic_font_color"].toString().toInt(ok, 16);
        hvr_bg_clr = obj["btn_non_transparent_std_pic_font_color"].toString().toInt(ok, 16);
        border_color = obj["btn_non_transparent_sltctd_border_color"].toString().toInt(ok, 16);
        sltctd_hvr_pic_font_color = obj["btn_transparent_std_pic_font_color"].toString().toInt(ok, 16);
        sltctd_std_pic_font_color = obj["btn_transparent_std_pic_font_color"].toString().toInt(ok, 16);
    }


    QColor tmp_std(std_pic_font_clr);
    QColor tmp_hvr(hvr_pic_font_clr);

    mid_r = std::min(tmp_std.red(), tmp_hvr.red()) + abs(tmp_std.red() - tmp_hvr.red()) / 2.0;
    mid_g = std::min(tmp_std.green(), tmp_hvr.green()) + abs(tmp_std.green() - tmp_hvr.green()) / 2.0;
    mid_b = std::min(tmp_std.blue(), tmp_hvr.blue()) + abs(tmp_std.blue() - tmp_hvr.blue()) / 2.0;
    update_qss();

    delete ok;
}

void AnimatedHoverButton::click() {
    this->is_clicked = true;
    std::swap(this->sltctd_std_pic_font_color, std_pic_font_clr);
    std::swap(this->sltctd_hvr_pic_font_color, hvr_pic_font_clr);
}

void AnimatedHoverButton::unclick() {
    this->is_clicked = false;
    std::swap(this->sltctd_std_pic_font_color, std_pic_font_clr);
    std::swap(this->sltctd_hvr_pic_font_color, hvr_pic_font_clr);
}
