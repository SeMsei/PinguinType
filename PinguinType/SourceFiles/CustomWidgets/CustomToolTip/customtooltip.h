
#ifndef CUSTOMTOOLTIP_H
#define CUSTOMTOOLTIP_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTimer>

enum ArrowPos{top, right, bottom, left, top_right, top_mid_right};


class CustomToolTip: public QFrame
{
    Q_OBJECT

public:


    CustomToolTip(QWidget *parent = nullptr);

    void set_text(QString str);
    void set_arrow_pos(ArrowPos arrow_pos);
    void set_pos(QPoint cur_pos);
    void update_size();

public slots:
    void custom_show();
    void custom_hide();

private slots:
    void anim_move();


protected:
    void paintEvent(QPaintEvent *e);

private:
    QString str;
    ArrowPos arrow_pos;
    QPoint cur_pos;
    QPoint tmp_pos;
    bool is_show_move = true;

    QTimer move_timer;

    int time = 300;

    QVBoxLayout *main_layout;
    QLineEdit *text;
};

#endif // CUSTOMTOOLTIP_H
