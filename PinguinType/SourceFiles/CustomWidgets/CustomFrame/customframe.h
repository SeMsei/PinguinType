
#ifndef CUSTOMFRAME_H
#define CUSTOMFRAME_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QJsonObject>
#include <QTimer>

class customframe : public QFrame
{
    Q_OBJECT
public:
    customframe(QWidget *parent = nullptr);
    ~customframe();
    void set_name(QString name);
    QString get_name();
    void select();
    void unselect();
    void set_theme(const QJsonObject& obj);

protected:
    bool eventFilter(QObject *obj, QEvent *e);
    void hover();
    void leave();

signals:
    void change_theme(QString theme_name);
    void tmp_change_theme(QString theme_name);
private slots:
    void emit_tmp_change_theme();

protected:
    void mouseReleaseEvent(QMouseEvent *e);

private:


    QHBoxLayout *main_layout, *right_main_layout, *left_layout, *right_layout;
    QFrame *right_main_frame, *left_frame, *right_frame;
    QString name;
    QLabel *check_mark_lbl, *theme_name_lbl,
        *first_clr, *second_clr, *third_clr;
    bool is_slctd = false;
    bool is_hovered = false;

    QRgb std_bg_color;
    QRgb font_color;
    QRgb hvr_bg_color;
    QRgb first_bg_color, second_bg_color, third_bg_color;
    QRgb right_bg_color;
    QTimer emit_timer;
};

#endif // CUSTOMFRAME_H
