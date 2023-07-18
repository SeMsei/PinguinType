#pragma once

#include <QObject>
#include <QTextBrowser>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <Qt>
#include <QTimer>
#include <QtCore/QObject>

#include "state.h"

class CustomTextBrowser:public QTextBrowser
{
    Q_OBJECT
public:
    void paint_text(QVector<QPair<QChar, State>> str);
    void set_str(QVector<QPair<QChar, State>> str);
    int get_font_size();
    void set_font_size(int font_size);
    void set_font_family(QString font_family);
    QString get_font_family();
    void set_pos(int pos);
    void set_text(QString text);
    void paint();
    bool is_move();

protected slots:
    void call_event();

protected:
    virtual void paintEvent(QPaintEvent *e) override;

private:
    int left_border_first = -1;
    int left_border_second = -1;
    int right_border_first = -1;
    int right_border_second = -1;
    int cur_pos = 0;
    int x = 20;
    int y = 20;
    int old_x;
    int new_x;

    QTextDocument td;

    QVector<QPair<QChar, State>> str;
    QString text;
    QString html;

    int font_size = 18;
    QString font_family = "Segoe UI";

    QTimer *tmr;
    bool is_moving = false;
    bool is_short = false;

    bool is_paint = false;
    bool is_paint_text = false;
};
