#ifndef LOGINNER_H
#define LOGINNER_H
#pragma once
#include <QMainWindow>
#include "main_dialog.h"
#include "client_connecter.h"
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class Loginner; }
QT_END_NAMESPACE

class Loginner : public QMainWindow
{
    Q_OBJECT

public:
    Loginner(QWidget *parent = nullptr);
    ~Loginner();

private slots:
    void on_pushButton_Login_clicked();

    void on_pushButton_2_Register_clicked();

private:
    Ui::Loginner *ui;
    Main_dialog& main_D_creator =  Main_dialog::get_instance();

    Client_connecter& connecter = Client_connecter::getInstance();
    int check_symbols(std::string username, std::string password);
};
#endif // LOGINNER_H
