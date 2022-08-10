#ifndef SECWINDOW_H
#define SECWINDOW_H

#include <QWidget>

namespace Ui {
class secWindow;
}

class secWindow : public QWidget
{
    Q_OBJECT

public:
    explicit secWindow(QWidget *parent = nullptr);
    ~secWindow();
    Ui::secWindow *ui;

private:


signals:
    void buttonPressed();

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_close_clicked();
};

#endif // SECWINDOW_H
