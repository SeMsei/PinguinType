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
    }

    if (e->type() == QEvent::HoverLeave) {
        StartHoverLeaveAnimation();
        is_enter = false;
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
    qss.append("border:0px solid black; border-radius: 10px;font: "+QString::number(font_size)+"px \"Comic Sans MS\";");
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
