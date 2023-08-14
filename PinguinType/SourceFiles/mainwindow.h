#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QMouseEvent>
#include <QDateTime>
#include <QTime>
#include <QTimer>
#include <QCheckBox>
#include <QPainter>
#include <QJsonObject>
#include <QShortcut>
#include <QKeySequence>
#include <QVBoxLayout>

#include "CustomWidgets/CustomTextBrowser/customtextbrowser.h"
#include "CustomWidgets/AnimatedHoverButton/animatedhoverbutton.h"
#include "CustomWidgets/CustomDialog/customdialog.h"
#include "CustomWidgets/CustomToolTip/customtooltip.h"


#pragma once

enum TypingMode {timer, word_count, quote};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CustomDialog;
class CustomToolTip;

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_tooltip();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_hide_window_btn_clicked();
    void on_close_window_btn_clicked();
    void change_time();
    void change_typing_time();
    void open_dialog();
    void theme(QString name);
    void tmp_theme(QString name);

    void main_login_btn_clicked();

    void start_test_btn_clicked();
    void restart_test();

private:
    void set_theme();

    void keyPressEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *e);
    void set_btn();
    void return_theme();

    void set_type_page_tt();

    Ui::MainWindow *ui;
    QString typing_string;
    QPoint cur_point;
    bool pressed = false;
    int h, m, s;
    CustomTextBrowser *type_tb;
    QVector<QPair<QChar, State>> str;

    int cur_pos = 0;
    int correct_pos = -1;
    int x = 0;
    int y = 0;

    QTimer typing_timer;

    bool is_typing = false;
    //make remember type
    //bool is_typing_on_time = false ;
    //int typing_limit = 15;
    int cur_word_typed = 0;
    int time_remaining = 15;
    int max_pos = 0;

    TypingMode cur_mode = TypingMode::timer;
    int time_limit = 15;
    int word_limit = 10;

    bool is_punctuation_on = false;
    bool is_numbers_on = false;

    bool is_tmp_theme = false;

    AnimatedHoverButton *signup_btn;
    AnimatedHoverButton *signin_btn;
    AnimatedHoverButton *start_test_btn;
    AnimatedHoverButton *leaderboard_btn;
    AnimatedHoverButton *info_btn;
    AnimatedHoverButton *settings_btn;
    AnimatedHoverButton *notif_btn;
    AnimatedHoverButton *signout_btn;
    AnimatedHoverButton *contact_btn;
    AnimatedHoverButton *support_btn;
    AnimatedHoverButton *terms_btn;
    AnimatedHoverButton *git_btn;
    AnimatedHoverButton *security_btn;
    AnimatedHoverButton *privacy_btn;
    AnimatedHoverButton *theme_btn;
    AnimatedHoverButton *version_btn;
    AnimatedHoverButton *frgt_psswd_btn;
    AnimatedHoverButton *avatar_btn;
    AnimatedHoverButton *restart_btn;
    QVBoxLayout *restart_layout;

    bool is_type_page = false, is_login_page = false, is_profile_page = false;

    CustomDialog *theme_dialog;

    CustomToolTip *start_test_tt;
    CustomToolTip *leaderboard_tt;
    CustomToolTip *info_tt;
    CustomToolTip *settings_tt;
    CustomToolTip *notif_tt;
    CustomToolTip *avatar_tt;
    CustomToolTip *restart_tt;

    QShortcut *restart_hk;

    QString cur_theme = "creamsicle";
};
