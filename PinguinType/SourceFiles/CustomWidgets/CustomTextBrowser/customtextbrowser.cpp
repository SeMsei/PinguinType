
#include "customtextbrowser.h"

void CustomTextBrowser::set_text(QString text) {
    this->text = text;
}

void CustomTextBrowser::set_str(QVector<QPair<QChar, State>> str) {
    this->str = str;
}

void CustomTextBrowser::paint_text(QVector<QPair<QChar, State>> str) {
    this->str = str;
    html = "<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }hr { height: 1px; border-width: 0; }li.unchecked::marker { content: \"\2610\"; }li.checked::marker { content: \"\2612\"; }</style></head><body style=\" font-family:'" + font_family +"'; font-size:" + QString::number(font_size) +"pt; font-weight:400; font-style:normal;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";

    for (size_t i = left_border_first; i <= right_border_first; i++) {
        if (str[i].second == State::Default) {
            html += "<span style=\"color:#000000;\">";
        } else if (str[i].second == State::Correct) {
            html += "<span style=\"color:#00FF00;\">";
        } else {
            html += "<span style=\"color:#FF0000;\">";
        }

        html += str[i].first;
        html += "</span>";
    }


    html.append("</p>");

    if (left_border_second != -1) {
        html.append("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">");
        for (size_t i = left_border_second; i <= right_border_second; i++) {
            if (str[i].second == State::Default) {
                html += "<span style=\"color:#000000;\">";
            } else if (str[i].second == State::Correct) {
                html += "<span style=\"color:#00FF00;\">";
            } else {
                html += "<span style=\"color:#FF0000;\">";
            }

            html += str[i].first;
            html += "</span>";
        }

        html.append("</p>");
    }

    html.append("</body></html>");

    td.setHtml(html);
}

void CustomTextBrowser::set_font_size(int font_size) {
    this->font_size = font_size;
}

int CustomTextBrowser::get_font_size() {
    return this->font_size;
}

bool CustomTextBrowser::is_move() {
    return is_moving;
}

void CustomTextBrowser::paint() {

    old_x = x;
    QFont f("Segoe UI", font_size);
    QFontMetrics fm(f);
    QString str = this->text.mid(left_border_first, cur_pos-left_border_first);
    int width = fm.horizontalAdvance(str);
    new_x = width+20;
    if (!is_moving) {
        is_moving = true;
        tmr = new QTimer();
        connect(tmr, SIGNAL(timeout()),this, SLOT(call_event()));
        tmr->setInterval(5);
        tmr->start();
    }
}

void CustomTextBrowser::call_event() {
    if (x == new_x) {
        is_short = false;
        x=new_x;
        update();
        delete tmr;
        is_moving = false;
    } else {
        is_short = true;

        if (new_x > old_x)
            x+=1;
        else
            x-=1;

        is_paint = true;
        update();
    }
}

void CustomTextBrowser::paintEvent(QPaintEvent *e) {
    QPainter painter(viewport());
    painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap));

    if (cur_pos == str.size()) {

        td.drawContents(&painter);
        is_paint_text = false;
    }

    if (is_short) {
        painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(x-16, y-9, x-16, y+font_size/3*4-10);
    } else {
        painter.setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(x-16,y-11,x-16, y+font_size/3*4-8);
    }

    td.drawContents(&painter);
}

void CustomTextBrowser::set_pos(int pos) {

    this->cur_pos = pos;

    if (cur_pos == str.size())
        return;


    QFont f("Segoe UI", font_size);
    QFontMetrics fm(f);
    QString strr = this->text.mid(left_border_first, cur_pos-left_border_first);

    //int width = fm.horizontalAdvance(strr);

    if (left_border_first == -1) {
        left_border_first = 0;
        QString tmp_str;
        size_t i;

        for (i = 0; i < str.size(); i++) {
            tmp_str += str[i].first;


            if (fm.horizontalAdvance(tmp_str) > this->size().width())
                break;

            if (str[i].first == ' ') {
                right_border_first = i;
                left_border_second = i + 1;
            }
        }

        if (i == str.size()) {
            right_border_first = i - 1;
            left_border_second = right_border_second = -1;

        } else {
            tmp_str.clear();

            for (size_t i = left_border_second; i < str.size(); i++) {
                tmp_str += str[i].first;

                if (fm.horizontalAdvance(tmp_str) >= this->size().width())
                    break;

                if (str[i].first == ' ')
                    right_border_second = i;
            }

            if (right_border_second == -1)
                right_border_second = str.size() - 1;
        }

    }

    if (cur_pos > right_border_first) {

        this->x = 20;

        left_border_first = left_border_second;
        right_border_first = right_border_second;

        if (right_border_first == str.size() - 1) {
            left_border_second = right_border_second = -1;
        } else {
            left_border_second = right_border_first + 1;
            QString tmp_str;
            size_t i;

            for (i = left_border_second; i < str.size(); i++) {
                tmp_str += str[i].first;

                if (fm.horizontalAdvance(tmp_str) >= this->size().width())
                    break;

                if (str[i].first == ' ')
                    right_border_second = i;

            }

            if (i == str.size())
                right_border_second = i - 1;
        }
    }
}

void CustomTextBrowser::set_font_family(QString font_family) {
    this->font_family = font_family;
}

QString CustomTextBrowser::get_font_family() {
    return this->font_family;
}
