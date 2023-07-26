#pragma once

#include <QPushButton>
#include <QColor>
#include <QPropertyAnimation>
#include <QPainter>

class QVariantAnimation;

class AnimatedHoverButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ get_pic_font_color WRITE set_pic_font_color)

public:
    explicit AnimatedHoverButton(QWidget *parent = 0);

    void set_bg_transparent(bool is_bg_transparent);
    void set_std_pic_font_clr(QRgb std_pic_font_clr);
    void set_std_bg_clr(QRgb std_bg_clr);
    void set_hvr_pic_font_clr(QRgb hvr_pic_font_clr);
    void set_hvr_bg_clr(QRgb hvr_bg_clr);
    void set_pic(QString pic_path);
    void update_qss();
    void set_font_size(int font_size);

protected:
    bool eventFilter(QObject *obj, QEvent *e);
    void set_pic_font_color(const QColor& color);
    const QColor& get_pic_font_color() const;

private:
    bool is_enter;
    bool is_paint_icon = false;

    QColor m_currentColor;
    QPropertyAnimation pic_font_clr_anim;
    QPropertyAnimation bg_clr_anim;

    QRgb std_pic_font_clr = 0x0;
    QRgb std_bg_clr = 0x0;
    QRgb hvr_pic_font_clr =0x0;
    QRgb hvr_bg_clr = 0x0;

    float mid_r = 0x8A;
    float mid_g = 0x8F;
    float mid_b = 0x96;
    QString pic_path = "";

    void StartHoverEnterAnimation();
    void StartHoverLeaveAnimation();

    QColor m_color;
    QColor m_bg_color;

    bool is_bg_transparent = false;

    QPixmap source_image;
    QPixmap changed_image;
    QIcon btn_icon;
    QPainter *painter;

    int font_size = 18;
};
