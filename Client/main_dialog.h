#ifndef MAIN_DIALOG_H
#define MAIN_DIALOG_H
#pragma once
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <string>


#include "text.h"
#include "builders_text.h"
#include "save_text.h"
#include "downloader.h"

#define PATHP "M:/7--------------QT------------------/Loginner"

namespace Ui {
class Main_dialog;
}

class Main_dialog : public QDialog
{
    Q_OBJECT

public:
    static Main_dialog& get_instance();

    std::string get_choice();
    void get_save_name();
    void sign(std::string file_name);
    void verify();
    void send_file();
    void recieve_file();

private slots:
    void on_Build_clicked();

    void on_pushButton_signOut_clicked();

    void on_pushButton_Upload_clicked();

    void on_pushButton_Download_clicked();

    void on_pushButton_clicked();

private:
    Ui::Main_dialog *ui;
    std::string path_project;

    Client_connecter& connecter = Client_connecter::getInstance();
    Builders_text& builder = Builders_text::getInstance();
    Downloader& downloader =  Downloader::getInstance();

    static Main_dialog* instance;
    explicit Main_dialog(QWidget *parent = nullptr);
    ~Main_dialog();

};

#endif // MAIN_DIALOG_H
