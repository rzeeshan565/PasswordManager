#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlistwidget.h"
#include <QMainWindow>
#include <secwindow.h>
#include<QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString file_loc = QString("%1/Pass_Mngr").arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    void save_data();
    void update_csv();
    QString item, email, password;

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_close_clicked();

    void update();

    void on_pushButton_show_clicked();


    void on_listWidget_itemDoubleClicked();

signals:
    void buttonPressed();

private:
    Ui::MainWindow *ui;
    secWindow *secDialog;
    QVector<QString> items;
    QVector<QVector<QString>> pass_man;
};
#endif // MAINWINDOW_H
