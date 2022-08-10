#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secwindow.h"
#include "ui_secwindow.h"
#include<qfile.h>
#include<QMessageBox>
#include <QDir>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!QDir(file_loc).exists()) QDir().mkdir(file_loc);
    LPCWSTR sw = (const wchar_t*) file_loc.utf16();
    SetFileAttributesW(sw, FILE_ATTRIBUTE_HIDDEN);
    QFile file(QString("%1/password_manager.csv").arg(file_loc));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Password Manager", "The file is created for you.");
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {

         QString line = in.readLine();
         QStringList list = line.split(",");
         items.push_back(list[0]);
         items.push_back(list[1]);
         items.push_back(list[2]);
         pass_man.push_back(items);
         items.clear();
    }
    for(int i=0; i<pass_man.size(); i++)
        ui->listWidget->addItem(pass_man[i][0]);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update() {

    item = secDialog->ui->lineEdit_title->text();
    email = secDialog->ui->lineEdit_email->text();
    password = secDialog->ui->lineEdit_password->text();
    bool exists = false;
    for(int i=0; i<pass_man.size(); i++)
        if(item == pass_man[i][0]) {
            pass_man[i][1] = email;
            pass_man[i][2] = password;
            update_csv();
            exists = true;
        }
    if(!exists) {
        items.push_back(item);
        items.push_back(email);
        items.push_back(password);
        pass_man.push_back(items);
        items.clear();
        save_data();
    }

    ui->listWidget->clear();
    for(int i=0; i<pass_man.size(); i++)
        ui->listWidget->addItem(pass_man[i][0]);
    delete secDialog;
}

void MainWindow::on_pushButton_add_clicked()
{
    secDialog = new secWindow();
    QObject::connect(secDialog, SIGNAL(buttonPressed()), this, SLOT(update()));
    secDialog->show();
}

void MainWindow::on_pushButton_close_clicked()
{  
    QApplication::quit();
}



void MainWindow::save_data() {

    QFile file(QString("%1/password_manager.csv").arg(file_loc));
    if(!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Password Manager", "File not found");
        return;
    }
    QTextStream out(&file);
    out << item << "," << email << "," << password << "\n";
    out.flush();
    file.close();
    QMessageBox::information(this, "Password Manager", "File Saved");

}

void MainWindow::update_csv() {

    QFile old_file(QString("%1/password_manager.csv").arg(file_loc));
    old_file.remove();
    QFile file(QString("%1/password_manager_new.csv").arg(file_loc));
    if(!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::warning(this, "Password Manager", "File not found");
        return;
    }
    QTextStream out(&file);
    for(int i=0; i<pass_man.size(); i++)
        out << pass_man[i][0] << "," << pass_man[i][1] << "," << pass_man[i][2] << "\n";

    out.flush();
    file.close();
    QFile::rename(QString("%1/password_manager_new.csv").arg(file_loc), QString("%1/password_manager.csv").arg(file_loc));
    QMessageBox::information(this, "Password Manager", "File Saved");


}

void MainWindow::on_pushButton_show_clicked()
{

    secDialog = new secWindow();
    for(int i=0; i<pass_man.size(); i++)
        if(ui->listWidget->currentItem()->text() == pass_man[i][0]) {
            secDialog->ui->lineEdit_title->setText(pass_man[i][0]);
            secDialog->ui->lineEdit_title->setReadOnly(true);
            QPalette *palette = new QPalette();
            palette->setColor(QPalette::Text,Qt::darkGray);
            secDialog->ui->lineEdit_title->setPalette(*palette);
            secDialog->ui->lineEdit_email->setText(pass_man[i][1]);
            secDialog->ui->lineEdit_password->setText(pass_man[i][2]);
        }
    QObject::connect(secDialog, SIGNAL(buttonPressed()), this, SLOT(update()));
    secDialog->show();
}


void MainWindow::on_listWidget_itemDoubleClicked()
{
    on_pushButton_show_clicked();
}

