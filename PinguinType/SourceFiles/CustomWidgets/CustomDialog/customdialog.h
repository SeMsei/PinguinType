#pragma once

#include <QDialog>
#include "../CustomFrame/customframe.h"
#include "../../mainwindow.h"

#pragma once

namespace Ui {
class CustomDialog;
}

class MainWindow;

class CustomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomDialog(QWidget *parent = nullptr);
    ~CustomDialog();

    void custom_hide();
    void set_theme(const QJsonObject &obj);


private slots:
    void on_theme_find_textChanged(const QString &arg1);

private:
    QVector<customframe*> theme_vec;
    Ui::CustomDialog *ui;
    MainWindow *parent;
};
