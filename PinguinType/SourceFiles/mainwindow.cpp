
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QToolTip>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   
    ui->setupUi(this);

    //set_theme();

    theme_dialog = new CustomDialog(this);

    this->setFocusPolicy(Qt::StrongFocus);

    this->ui->move_frame->setFocusPolicy(Qt::NoFocus);

    this->ui->logo_btn->setCursor(Qt::PointingHandCursor);
    this->ui->move_frame->installEventFilter(this);
    this->ui->typing_frame->installEventFilter(this);
    QTime tmp =  QTime::currentTime();
    this->ui->time_label->setText(QString::number(tmp.hour()) + ":" +
                                  QString::number(tmp.minute()) + ":" +
                                  QString::number(tmp.second()));

    QTimer *main_timer = new QTimer();
    connect(main_timer, SIGNAL(timeout()), this, SLOT(change_time()));
    main_timer->start(1000);

    this->type_tb = new CustomTextBrowser();
    this->type_tb->setMinimumSize(QSize(this->size().width()*0.9, 100));
    this->type_tb->setMaximumSize(QSize(this->size().width()*0.9, 100));
    type_tb->setParent(this);

    this->type_tb->setEnabled(false);
    //this->type_tb->setAttribute(Qt::WA_OpaquePaintEvent);
    QVBoxLayout *typing_layout = new QVBoxLayout();

    this->ui->typing_frame->setLayout(typing_layout);
    typing_layout->addWidget(type_tb);
    typing_layout->setAlignment(Qt::AlignTop);

    typing_string = "один два три четыре пять шесь, семь восемь. Девять! 123123123 123123123 123123123 12312312 3123123123 123123"; //прпрпрпрп прпрпр прпрпрп рпрпрп пррпрп рпр рпрпр рпрпр
    for (size_t i = 0; i < typing_string.size(); i++) {
        str.push_back(QPair<QChar, State>({typing_string[i], State::Default}));
    }

    type_tb->set_text(typing_string);
    type_tb->setText(typing_string);

    this->type_tb->set_str(str);
    this->type_tb->set_pos(0);


    typing_timer.setInterval(1000);
    connect(&typing_timer, SIGNAL(timeout()), this, SLOT(change_typing_time()));


    this->set_btn();
    this->set_theme();

    this->type_tb->paint_text(str);
    this->type_tb->update();

    restart_hk = new QShortcut(QKeySequence(Qt::SHIFT | Qt::Key_Tab), this);
    connect(restart_hk, SIGNAL(activated()), this, SLOT(restart_test()));


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_hide_window_btn_clicked()
{
    this->showMinimized();
}


void MainWindow::on_close_window_btn_clicked()
{
    QApplication::quit();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    cur_point = event->pos();

    if (!theme_dialog->isHidden()) {
        theme_dialog->custom_hide();

        if (is_tmp_theme)
            return_theme();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    if(pressed)
        this->move(mapToParent(event->pos() - cur_point));
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress){
        //qDebug() << object->objectName();
    }
    if (object == ui->move_frame && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = (QMouseEvent*)event;
        if (pressed == false){
            cur_point = mouseEvent->pos();
        }
        pressed = true;
        return true;
    }
    if (object == ui->move_frame && event->type() == QEvent::MouseButtonRelease)
    {
        pressed = false;
        return true;
    }
    else
        return false;
}

void MainWindow::change_time() {
    QTime tmp =  QTime::currentTime();
    QString s = QString::number(tmp.second());
    QString m = QString::number(tmp.minute());
    QString h = QString::number(tmp.hour());

    if (s.size() == 1)
        s.push_front('0');

    if (m.size() == 1)
        m.push_front('0');

    if (h.size() == 1)
        h.push_front('0');

    this->ui->time_label->setText(h + ":" + m + ":" + s);
}


void MainWindow::main_login_btn_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::start_test_btn_clicked()
{


    //get new typing_string
    str.clear();
    cur_pos = 0;
    correct_pos = -1;
    type_tb->reset();

    for (size_t i = 0; i < typing_string.size(); i++) {
        str.push_back(QPair<QChar, State>({typing_string[i], State::Default}));
    }
    type_tb->set_text(typing_string);
    type_tb->setText(typing_string);

    this->type_tb->set_str(str);
    this->type_tb->set_pos(0);
    this->type_tb->paint_text(str);
    this->type_tb->update();

    if (this->typing_timer.isActive())
        this->typing_timer.stop();

    cur_word_typed = 0;
    time_remaining = time_limit;

    this->ui->label->setText("");
    is_typing = false;

    this->ui->stackedWidget->setCurrentIndex(1);
    this->max_pos = 0;

    if (!is_type_page) {
        set_type_page_tt();
        is_type_page = true;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (this->ui->stackedWidget->currentIndex() != 1)
        return;

    this->max_pos = qMax(max_pos, cur_pos);

    if (event->key() == Qt::Key_Backspace) {
        if (cur_pos - correct_pos > 1 && cur_pos > 0) {
            cur_pos--;
            str[cur_pos].second = State::Default;
            this->type_tb->set_pos(cur_pos);
            this->type_tb->paint_text(str);

            this->type_tb->paint();

            return;
        }
    }

    if (event->text().size() == 1 && event->key() != Qt::Key_Delete &&
        event->key() != Qt::Key_Escape && event->key() != Qt::Key_Backspace &&
        event->key() != Qt::Key_Tab && event->key() != Qt::Key_Enter) {

        //qDebug() << cur_pos;

        if (!is_typing && cur_mode == TypingMode::timer) {
            this->typing_timer.start();
            is_typing = true;
            this->ui->label->setText(QString::number(time_remaining));
        }

        if (!is_typing && cur_mode == TypingMode::word_count) {
            is_typing = true;
            this->ui->label->setText(QString("0/") + QString::number(word_limit));
        }

        if (cur_pos == str.size() - 1) {
            //when typing on time end will never be reached
            //only when typing on word count
            //count_statistics()
            this->ui->label->setText(QString::number(word_limit) + "/" + QString::number(word_limit));
            this->cur_word_typed = 0;
            //wait
            this->ui->label->setText("");
            this->ui->stackedWidget->setCurrentIndex(0);
            return;
        }

        QChar entered_char = event->text().front();
        QChar text_char = this->str[cur_pos].first;//ui->textBrowser->toPlainText()[this->cur_pos];

        if (text_char == entered_char) {
            if (text_char == ' ' && cur_mode == TypingMode::word_count && max_pos == cur_pos) {
                cur_word_typed++;
                this->ui->label->setText(QString::number(cur_word_typed) + QString("/")+ QString::number(word_limit));
            }

            if (cur_pos - 1 == correct_pos)
                correct_pos++;

            str[cur_pos].second = State::Correct;
            this->cur_pos++;
            this->type_tb->set_pos(cur_pos);
            this->type_tb->paint_text(str);
            this->type_tb->paint();


        } else {
            if (text_char == ' ')
                return;

            str[cur_pos].second = State::Wrong;
            this->cur_pos++;
            this->type_tb->set_pos(cur_pos);
            this->type_tb->paint_text(str);
            this->type_tb->paint();
        }
    }
}

void MainWindow::change_typing_time() {
    time_remaining--;
    this->ui->label->setText(QString::number(time_remaining));

//    QTimer tmp;
//    tmp.setInterval(100);
//    tmp.setSingleShot(true);
//    tmp.

    if (time_remaining == 0) {
        //count statistics
        is_typing = false;
        time_remaining = time_limit; //delete it, it always reset by clicking start_test_btn
        this->ui->stackedWidget->setCurrentIndex(0);
        this->typing_timer.stop();
        this->ui->label->setText("");
    }
}

void MainWindow::set_btn() {
    //signup
    signup_btn = new AnimatedHoverButton(this);
    signup_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    signup_btn->set_bg_transparent(false);
    signup_btn->set_std_pic_font_clr(0xE9ECF0);
    signup_btn->set_hvr_pic_font_clr(0x2F3134);
    signup_btn->setText("sign up");
    signup_btn->update_qss();
    signup_btn->setMinimumHeight(35);
    ui->register_form->layout()->addWidget(signup_btn);
    signup_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //signin
    signin_btn = new AnimatedHoverButton(this);
    signin_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    signin_btn->set_bg_transparent(false);
    signin_btn->set_std_pic_font_clr(0xE9ECF0);
    signin_btn->set_hvr_pic_font_clr(0x2F3134);
    signin_btn->setText("sign in");
    signin_btn->update_qss();
    signin_btn->setMinimumHeight(35);
    ui->login_form->layout()->addWidget(signin_btn);
    signin_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //start_test
    start_test_btn = new AnimatedHoverButton(this);
    start_test_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    start_test_btn->set_bg_transparent(true);
    start_test_btn->set_std_pic_font_clr(0x939EAE);
    start_test_btn->set_hvr_pic_font_clr(0xE9ECF0);
    start_test_btn->set_pic(":images/icons/icons/keyboard.png");
    start_test_btn->setIconSize(QSize(30, 30));
    start_test_btn->update_qss();
    ui->navigation_left_bar_frame->layout()->addWidget(start_test_btn);
    connect(start_test_btn, SIGNAL(clicked()), this, SLOT(start_test_btn_clicked()));
    start_test_btn->setFocusPolicy(Qt::NoFocus);


    //leaderboard
    leaderboard_btn = new AnimatedHoverButton(this);
    leaderboard_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    leaderboard_btn->set_bg_transparent(true);
    leaderboard_btn->set_std_pic_font_clr(0x939EAE);
    leaderboard_btn->set_hvr_pic_font_clr(0xE9ECF0);
    leaderboard_btn->set_pic(":images/icons/icons/crown.png");
    leaderboard_btn->setIconSize(QSize(30, 30));
    leaderboard_btn->update_qss();
    ui->navigation_left_bar_frame->layout()->addWidget(leaderboard_btn);
    leaderboard_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //info_btn
    info_btn = new AnimatedHoverButton(this);
    info_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    info_btn->set_bg_transparent(true);
    info_btn->set_std_pic_font_clr(0x939EAE);
    info_btn->set_hvr_pic_font_clr(0xE9ECF0);
    info_btn->set_pic(":images/icons/icons/information.png");
    info_btn->setIconSize(QSize(30, 30));
    info_btn->update_qss();
    ui->navigation_left_bar_frame->layout()->addWidget(info_btn);
    info_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //settings_btn
    settings_btn = new AnimatedHoverButton(this);
    settings_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    settings_btn->set_bg_transparent(true);
    settings_btn->set_std_pic_font_clr(0x939EAE);
    settings_btn->set_hvr_pic_font_clr(0xE9ECF0);
    settings_btn->set_pic(":images/icons/icons/settings.png");
    settings_btn->setIconSize(QSize(30, 30));
    settings_btn->update_qss();
    ui->navigation_left_bar_frame->layout()->addWidget(settings_btn);
    settings_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //frgt_psswd_btn
    frgt_psswd_btn = new AnimatedHoverButton(this);
    frgt_psswd_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    frgt_psswd_btn->set_bg_transparent(true);
    frgt_psswd_btn->set_std_pic_font_clr(0x939EAE);
    frgt_psswd_btn->set_hvr_pic_font_clr(0xE9ECF0);
    frgt_psswd_btn->setText("forgot password?");
    frgt_psswd_btn->set_font_size(12);
    frgt_psswd_btn->update_qss();
    ui->frame->layout()->addWidget(frgt_psswd_btn);
    frgt_psswd_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //notif_btn
    notif_btn = new AnimatedHoverButton(this);
    notif_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    notif_btn->set_bg_transparent(true);
    notif_btn->set_std_pic_font_clr(0x939EAE);
    notif_btn->set_hvr_pic_font_clr(0xE9ECF0);
    notif_btn->set_pic(":images/icons/icons/bell.png");
    notif_btn->setIconSize(QSize(30, 30));
    notif_btn->update_qss();
    ui->navigation_right_bar_frame->layout()->addWidget(notif_btn);
    notif_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //avatar_btn
    avatar_btn = new AnimatedHoverButton(this);
    avatar_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    avatar_btn->set_bg_transparent(true);
    avatar_btn->set_std_pic_font_clr(0x939EAE);
    avatar_btn->set_hvr_pic_font_clr(0xE9ECF0);
    avatar_btn->set_pic(":images/icons/icons/avatarw.png");
    avatar_btn->setIconSize(QSize(30, 30));
    avatar_btn->update_qss();
    ui->navigation_right_bar_frame->layout()->addWidget(avatar_btn);
    connect(avatar_btn, SIGNAL(clicked()), this, SLOT(main_login_btn_clicked()));
    avatar_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //contact_btn
    contact_btn = new AnimatedHoverButton(this);
    contact_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    contact_btn->set_bg_transparent(true);
    contact_btn->set_std_pic_font_clr(0x939EAE);
    contact_btn->set_hvr_pic_font_clr(0xE9ECF0);
    contact_btn->set_pic(":images/icons/icons/mail.png");
    contact_btn->setIconSize(QSize(18, 18));
    contact_btn->set_font_size(14);
    contact_btn->update_qss();
    contact_btn->setText("contact");
    ui->left_info_frame->layout()->addWidget(contact_btn);
    contact_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //support_btn
    support_btn = new AnimatedHoverButton(this);
    support_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    support_btn->set_bg_transparent(true);
    support_btn->set_std_pic_font_clr(0x939EAE);
    support_btn->set_hvr_pic_font_clr(0xE9ECF0);
    support_btn->set_pic(":images/icons/icons/donatew.png");
    support_btn->setIconSize(QSize(18, 18));
    support_btn->set_font_size(14);
    support_btn->update_qss();
    support_btn->setText("support");
    ui->left_info_frame->layout()->addWidget(support_btn);
    support_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //git_btn
    git_btn = new AnimatedHoverButton(this);
    git_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    git_btn->set_bg_transparent(true);
    git_btn->set_std_pic_font_clr(0x939EAE);
    git_btn->set_hvr_pic_font_clr(0xE9ECF0);
    git_btn->set_pic(":images/icons/icons/codew.png");
    git_btn->setIconSize(QSize(18, 18));
    git_btn->set_font_size(14);
    git_btn->update_qss();
    git_btn->setText("github");
    ui->left_info_frame->layout()->addWidget(git_btn);
    git_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //terms_btn
    terms_btn = new AnimatedHoverButton(this);
    terms_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    terms_btn->set_bg_transparent(true);
    terms_btn->set_std_pic_font_clr(0x939EAE);
    terms_btn->set_hvr_pic_font_clr(0xE9ECF0);
    terms_btn->set_pic(":images/icons/icons/documentsw.png");
    terms_btn->setIconSize(QSize(18, 18));
    terms_btn->set_font_size(14);
    terms_btn->update_qss();
    terms_btn->setText("terms");
    ui->left_info_frame->layout()->addWidget(terms_btn);
    terms_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //security_btn
    security_btn = new AnimatedHoverButton(this);
    security_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    security_btn->set_bg_transparent(true);
    security_btn->set_std_pic_font_clr(0x939EAE);
    security_btn->set_hvr_pic_font_clr(0xE9ECF0);
    security_btn->set_pic(":images/icons/icons/shieldw.png");
    security_btn->setIconSize(QSize(18, 18));
    security_btn->set_font_size(14);
    security_btn->update_qss();
    security_btn->setText("security");
    ui->left_info_frame->layout()->addWidget(security_btn);
    security_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //privacy_btn
    privacy_btn = new AnimatedHoverButton(this);
    privacy_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    privacy_btn->set_bg_transparent(true);
    privacy_btn->set_std_pic_font_clr(0x939EAE);
    privacy_btn->set_hvr_pic_font_clr(0xE9ECF0);
    privacy_btn->set_pic(":images/icons/icons/padlockw.png");
    privacy_btn->setIconSize(QSize(18, 18));
    privacy_btn->set_font_size(14);
    privacy_btn->update_qss();
    privacy_btn->setText("privacy");
    ui->left_info_frame->layout()->addWidget(privacy_btn);
    privacy_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //theme_btn
    theme_btn = new AnimatedHoverButton(this);
    theme_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    theme_btn->set_bg_transparent(true);
    theme_btn->set_std_pic_font_clr(0x939EAE);
    theme_btn->set_hvr_pic_font_clr(0xE9ECF0);
    theme_btn->set_pic(":images/icons/icons/watercolorw.png");
    theme_btn->setIconSize(QSize(18, 18));
    theme_btn->set_font_size(14);
    theme_btn->update_qss();
    theme_btn->setText("theme");
    ui->right_info_frame->layout()->addWidget(theme_btn);
    connect(theme_btn, SIGNAL(clicked()), this, SLOT(open_dialog()));
    theme_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //version_btn
    version_btn = new AnimatedHoverButton(this);
    version_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    version_btn->set_bg_transparent(true);
    version_btn->set_std_pic_font_clr(0x939EAE);
    version_btn->set_hvr_pic_font_clr(0xE9ECF0);
    version_btn->set_pic(":images/icons/icons/gitw.png");
    version_btn->setIconSize(QSize(18, 18));
    version_btn->set_font_size(14);
    version_btn->update_qss();
    version_btn->setText("version");
    ui->right_info_frame->layout()->addWidget(version_btn);
    version_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    //restart_btn
    restart_btn = new AnimatedHoverButton(this);
    restart_btn->setCursor(Qt::CursorShape::PointingHandCursor);
    restart_btn->set_bg_transparent(true);
    restart_btn->set_std_pic_font_clr(0x939EAE);
    restart_btn->set_hvr_pic_font_clr(0xE9ECF0);
    restart_btn->set_pic(":images/icons/icons/restart.png");
    restart_btn->setIconSize(QSize(18, 18));
    restart_btn->set_font_size(14);
    restart_btn->update_qss();
    restart_btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    ui->restart_frame->layout()->addWidget(restart_btn);
    ui->restart_frame->layout()->setAlignment(Qt::AlignHCenter);
    connect(restart_btn, SIGNAL(clicked()), this, SLOT(restart_test()));

}

void MainWindow::set_theme() {
    QString val;
    QFile file;
    file.setFileName(QDir::currentPath() + "/themes.json");

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

    const QJsonObject &object = d.object();

    QJsonObject cur = object["current"].toObject();

    theme_dialog->set_theme(d.object());

    //hotkeys
    this->ui->restart_hk_1->setStyleSheet("color: #"+cur["main_bg_color"].toString()+";"
                                          "font: 12px \"Comic Sans MS\";"
                                          "background-color: #"+cur["btn_non_transparent_std_pic_font_color"].toString()+";"
                                          "border-radius: 2px;");
    this->ui->restart_hk_2->setStyleSheet("color: #"+cur["btn_non_transparent_std_pic_font_color"].toString()+";"
                                          "font: 12px \"Comic Sans MS\";"
                                          "background-color: transparent;"
                                          "border-radius: 2px;");
    this->ui->restart_hk_3->setStyleSheet("color: #"+cur["main_bg_color"].toString()+";"
                                          "font: 12px \"Comic Sans MS\";"
                                          "background-color: #"+cur["btn_non_transparent_std_pic_font_color"].toString()+";"
                                          "border-radius: 2px;");
    this->ui->restart_hk_4->setStyleSheet("color: #"+cur["btn_non_transparent_std_pic_font_color"].toString()+";"
                                          "font: 12px \"Comic Sans MS\";"
                                          "background-color: transparent;"
                                          "border-radius: 2px;");

    //buttons
    signup_btn->set_theme(cur);
    signin_btn->set_theme(cur);
    start_test_btn->set_theme(cur);
    leaderboard_btn->set_theme(cur);
    info_btn->set_theme(cur);
    settings_btn->set_theme(cur);
    notif_btn->set_theme(cur);
    contact_btn->set_theme(cur);
    support_btn->set_theme(cur);
    terms_btn->set_theme(cur);
    git_btn->set_theme(cur);
    security_btn->set_theme(cur);
    privacy_btn->set_theme(cur);
    theme_btn->set_theme(cur);
    version_btn->set_theme(cur);
    frgt_psswd_btn->set_theme(cur);
    avatar_btn->set_theme(cur);
    restart_btn->set_theme(cur);

    signup_btn->update_qss();
    signin_btn->update_qss();
    start_test_btn->update_qss();
    leaderboard_btn->update_qss();
    info_btn->update_qss();
    settings_btn->update_qss();
    notif_btn->update_qss();
    contact_btn->update_qss();
    support_btn->update_qss();
    terms_btn->update_qss();
    git_btn->update_qss();
    security_btn->update_qss();
    privacy_btn->update_qss();
    theme_btn->update_qss();
    version_btn->update_qss();
    frgt_psswd_btn->update_qss();
    avatar_btn->update_qss();
    restart_btn->update_qss();

    //main
    bool *ok = new bool();
    QString qss;
    qss.append("QFrame#main_frame { border:4px solid #"+cur["main_border_color"].toString()+
               ";border-radius:20px; background-color:#"+cur["main_bg_color"].toString()+";}");
    this->ui->main_frame->setStyleSheet(qss);

    qss.clear();
    qss.append("font: 30px \"Comic Sans MS\";color:#"+cur["logo_font_color"].toString()+";"
               "background-color: transparent;padding-left: 5px;"
               "padding-top: 0px;padding-bottom: 0px;padding-right: 0px;");
    this->ui->logo_btn->setStyleSheet(qss);

    qss.clear();
    qss.append("font: 18px \"Comic Sans MS\";color:#"+cur["timer_font_color"].toString()+";");
    this->ui->time_label->setStyleSheet(qss);

    QPixmap logo(":images/icons/icons/PinguinIcon.png");
    QPainter painter(&logo);
    QRgb base_color = cur["logo_icon_color"].toString().toInt(ok, 16);

    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//::CompositionMode_SourceIn
    painter.fillRect(logo.rect(), base_color);
    painter.end();
    QIcon btn_icn;
    btn_icn.addPixmap(logo);
    this->ui->pushButton->setIcon(btn_icn);

    qss.clear();
    qss.append("font: 18px \"Comic Sans MS\";color:#"+cur["line_edit_text_color"].toString()+";"
            "border-radius:10px;border:0px;background-color:#"+cur["line_edit_bg_color"].toString()+";"
            "padding-left: 5px;selection-color:#"+cur["highlighted_text_color"].toString()+";"
            "selection-background-color:#"+cur["highlight_text_color"].toString()+";");
    this->ui->username_input->setStyleSheet(qss);
    this->ui->email_input->setStyleSheet(qss);
    this->ui->login_username_input->setStyleSheet(qss);
    this->ui->login_password_input->setStyleSheet(qss);
    this->ui->verify_password_input->setStyleSheet(qss);
    this->ui->verify_email_input->setStyleSheet(qss);
    this->ui->password_input->setStyleSheet(qss);

    qss.clear();
    qss.append("font: 18px \"Comic Sans MS\";"
               "color:#"+cur["register_login_font_color"].toString()+";"
               "padding-left:2px;");

    this->ui->register_label->setStyleSheet(qss);
    this->ui->login_label->setStyleSheet(qss);

    qss.clear();
    qss.append("QCheckBox {font: 18px \"Comic Sans MS\";color:#"+cur["checkbox_font_color"].toString()+";}"
"QCheckBox::indicator {width: 20px;height: 20px;background-color:#"+cur["checkbox_bg_color"].toString()+";"
"border-radius: 4px;border-style: solid;border-width: 0px;border-color: white white black black;}"
                                                       "QCheckBox::indicator:checked {image: url(\":/images/marks/check_marks/"+cur["check_mark_color"].toString()+".png\");}");

    this->ui->remem_checkbox->setStyleSheet(qss);

    qss.clear();
    qss.append("QPushButton#hide_window_btn  {background-color:transparent;}"
"QPushButton#hide_window_btn::hover {background-color:#"+cur["minimize_bg_hvr_color"].toString()+";"
                                                           "border-radius:7px;}");
    this->ui->hide_window_btn->setStyleSheet(qss);

    qss.clear();
    qss.append("font: 18px \"Comic Sans MS\";color:#"+cur["timer_font_color"].toString()+";");
    this->ui->time_label->setStyleSheet(qss);
    //qDebug() << d.toJson(QJsonDocument::Indented).toStdString().c_str();
    this->type_tb->set_default_color(cur["default_text_color"].toString().toInt(ok, 16));
    this->type_tb->set_caret_color(cur["caret_text_color"].toString().toInt(ok, 16));
    type_tb->set_correct_color(cur["correct_text_color"].toString().toInt(ok, 16));
    type_tb->set_incorrect_color(cur["incorrect_text_color"].toString().toInt(ok, 16));
    type_tb->setStyleSheet("background-color:#"+cur["text_browser_bg_color"].toString()+";border-radius:20px;");
    type_tb->paint_text(this->str);
    delete ok;
}

void MainWindow::open_dialog() {
    theme_dialog->show();

    QGraphicsOpacityEffect *fade_effect = new QGraphicsOpacityEffect(theme_dialog);
    theme_dialog->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animation = new QPropertyAnimation(fade_effect, "opacity");
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->setDuration(150);
    theme_dialog->setGraphicsEffect(fade_effect);
    animation->setStartValue(0.01);
    animation->setEndValue(1.0);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e) {
    if (theme_dialog && !theme_dialog->isHidden()) {
        theme_dialog->custom_hide();

        if (is_tmp_theme)
            return_theme();
    }
}

void MainWindow::theme(QString name) {
    QString val;
    QFile file;
    file.setFileName(QDir::currentPath() + "/themes.json");

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject obj = d.object();

    auto find_it = obj.find("current");
    obj.erase(find_it);
    obj.insert("current", obj[name]);
    obj.erase(obj.find("current_name"));
    obj.insert("current_name", QJsonValue(name));

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    file.close();
    theme_dialog->custom_hide();

    this->set_theme();
    //std::cout << QJsonDocument(obj).toJson(QJsonDocument::Indented).toStdString();

}

void MainWindow::tmp_theme(QString name) {
    QString val;
    QFile file;
    file.setFileName(QDir::currentPath() + "/themes.json");

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject obj = d.object();

    auto find_it = obj.find("current");
    obj.erase(find_it);
    obj.insert("current", obj[name]);
    is_tmp_theme = true;

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    file.close();

    this->set_theme();
}

void MainWindow::return_theme() {
    is_tmp_theme = false;

    QString val;
    QFile file;
    file.setFileName(QDir::currentPath() + "/themes.json");

    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject obj = d.object();

    auto find_it = obj.find("current");
    obj.erase(find_it);
    obj.insert("current", obj[obj["current_name"].toString()]);

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    file.close();

    this->set_theme();
}

void MainWindow::set_tooltip() {
    //start_test_tt
    start_test_tt = new CustomToolTip(this);
    start_test_tt->set_text("start test");
    start_test_tt->set_arrow_pos(ArrowPos::top);
    connect(start_test_btn, SIGNAL(hover_signal()), start_test_tt, SLOT(custom_show()));
    connect(start_test_btn, SIGNAL(leave_signal()), start_test_tt, SLOT(custom_hide()));
    start_test_tt->hide();
    start_test_tt->update_size();
    double x = start_test_btn->mapTo(this, QPointF(0,0)).rx();
    x += start_test_btn->size().width() / 2.0;
    x -= start_test_tt->size().width() / 2.0;
    x-=2;
    double y = start_test_btn->mapTo(this, QPointF(0,0)).ry();
    y += start_test_btn->height()+10;

    start_test_tt->set_pos({(int)x,(int)y});

    //leaderboard_tt
    leaderboard_tt = new CustomToolTip(this);
    leaderboard_tt->set_text("leaderboard");
    leaderboard_tt->set_arrow_pos(ArrowPos::top);
    connect(leaderboard_btn, SIGNAL(hover_signal()), leaderboard_tt, SLOT(custom_show()));
    connect(leaderboard_btn, SIGNAL(leave_signal()), leaderboard_tt, SLOT(custom_hide()));
    leaderboard_tt->hide();
    leaderboard_tt->update_size();
    x = leaderboard_btn->mapTo(this, QPointF(0,0)).rx();
    x += leaderboard_btn->size().width() / 2.0;
    x -= leaderboard_tt->size().width() / 2.0;
    x-=2;
    y = leaderboard_btn->mapTo(this, QPointF(0,0)).ry();
    y += leaderboard_btn->height()+10;
    leaderboard_tt->set_pos({(int)x,(int)y});

    //info_tt
    info_tt = new CustomToolTip(this);
    info_tt->set_text("info");
    info_tt->set_arrow_pos(ArrowPos::top);
    connect(info_btn, SIGNAL(hover_signal()), info_tt, SLOT(custom_show()));
    connect(info_btn, SIGNAL(leave_signal()), info_tt, SLOT(custom_hide()));
    info_tt->hide();
    info_tt->update_size();
    x = info_btn->mapTo(this, QPointF(0,0)).rx();
    x += info_btn->size().width() / 2.0;
    x -= info_tt->size().width() / 2.0;
    x-=2;
    y = info_btn->mapTo(this, QPointF(0,0)).ry();
    y += info_btn->height()+10;
    info_tt->set_pos({(int)x,(int)y});

    //settings_tt
    settings_tt = new CustomToolTip(this);
    settings_tt->set_text("settings");
    settings_tt->set_arrow_pos(ArrowPos::top);
    connect(settings_btn, SIGNAL(hover_signal()), settings_tt, SLOT(custom_show()));
    connect(settings_btn, SIGNAL(leave_signal()), settings_tt, SLOT(custom_hide()));
    settings_tt->hide();
    settings_tt->update_size();
    x = settings_btn->mapTo(this, QPointF(0,0)).rx();
    x += settings_btn->size().width() / 2.0;
    x -= settings_tt->size().width() / 2.0;
    x-=2;
    y = settings_btn->mapTo(this, QPointF(0,0)).ry();
    y += settings_btn->height()+10;
    settings_tt->set_pos({(int)x,(int)y});

    //notif_tt
    notif_tt = new CustomToolTip(this);
    notif_tt->set_text("notifications");
    notif_tt->set_arrow_pos(ArrowPos::top_mid_right);
    connect(notif_btn, SIGNAL(hover_signal()), notif_tt, SLOT(custom_show()));
    connect(notif_btn, SIGNAL(leave_signal()), notif_tt, SLOT(custom_hide()));
    notif_tt->hide();
    notif_tt->update_size();

    x = notif_btn->mapTo(this, QPointF(0,0)).rx();
    x += notif_btn->size().width() / 2.0;
    x -= notif_tt->size().width() / 4.0 * 3.0;
    x+=5;
    y = notif_btn->mapTo(this, QPointF(0,0)).ry();
    y += notif_btn->height()+10;
    notif_tt->set_pos({(int)x,(int)y});


    //avatar_tt
    avatar_tt = new CustomToolTip(this);
    avatar_tt->set_text("notifications");
    avatar_tt->set_arrow_pos(ArrowPos::top_right);
    connect(avatar_btn, SIGNAL(hover_signal()), avatar_tt, SLOT(custom_show()));
    connect(avatar_btn, SIGNAL(leave_signal()), avatar_tt, SLOT(custom_hide()));
    avatar_tt->hide();
    avatar_tt->update_size();
    x = avatar_btn->mapTo(this, QPointF(0,0)).rx();
    x += avatar_btn->size().width();
    x -= avatar_tt->size().width();
    x -= 5 ;
    y = avatar_btn->mapTo(this, QPointF(0,0)).ry();
    y += avatar_btn->height()+10;
    avatar_tt->set_pos({(int)x,(int)y});

    //restart_tt

}

void MainWindow::restart_test() {
    if (this->ui->stackedWidget->currentIndex() == 1) {
        this->start_test_btn_clicked();
    }
}

void MainWindow::set_type_page_tt() {
    restart_tt = new CustomToolTip(this);
    restart_tt->set_text("restart");
    restart_tt->set_arrow_pos(ArrowPos::top);
    connect(restart_btn, SIGNAL(hover_signal()), restart_tt, SLOT(custom_show()));
    connect(restart_btn, SIGNAL(leave_signal()), restart_tt, SLOT(custom_hide()));
    restart_tt->hide();
    restart_tt->update_size();
    x = restart_btn->mapTo(this, QPointF(0,0)).rx();
    x += restart_btn->size().width() / 2.0;
    x -= restart_tt->size().width() / 2.0;
    x-=2;
    y = restart_btn->mapTo(this, QPointF(0,0)).ry();
    y += restart_btn->height()+10;
    restart_tt->set_pos({(int)x,(int)y});
    this->restart_btn->update();
    qDebug() << x << y << restart_btn->mapTo(this, QPointF(0,0)) << restart_btn->size() << restart_tt->size();
}
