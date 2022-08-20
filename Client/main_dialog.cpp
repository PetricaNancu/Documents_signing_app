#include "main_dialog.h"
#include "ui_main_dialog.h"
#include <QMessageBox>
#include <QPixmap>
#include <fstream>


#include "loginner.h"

Loginner *logger;
Main_dialog::Main_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Main_dialog)
{
    this->path_project = PATHP;
    ui->setupUi(this);
    QPixmap pix_background("M:/7--------------QT------------------/Loginner/poze/test_6.jpg");
    ui->background->setPixmap(pix_background);
    QPixmap pix_logo("M:/7--------------QT------------------/Loginner/poze/emblem#3.png");
    ui->Logo->setPixmap(pix_logo);
}

Main_dialog::~Main_dialog()
{
    delete ui;
}
Main_dialog * Main_dialog::instance =nullptr;
Main_dialog& Main_dialog::get_instance()
{
    if (Main_dialog::instance==nullptr)
    {
        instance = new Main_dialog();
    }
    return *instance;
}
std::string Main_dialog::get_choice()
{
    QString aux =ui->EncryptDecrypt->currentText();
    return aux.toStdString();
}
void Main_dialog::send_file()
{
    Text& text_obj_creator=Text::getInstance();
    std::string aux=text_obj_creator.get_text();
    std::ofstream creator;
    creator.open("M:/7--------------QT------------------/Loginner/file.txt");
    creator<<aux;
    creator.close();

    std::string message ;
    std::ifstream reader;
    reader.open("M:/7--------------QT------------------/Loginner/file.txt");
    std::string line;
       while (std::getline(reader, line))
       {
           message= connecter.recieve_message();
           if(message == "ok")
              { line.append("\n");
               connecter.send_message(line);}

       }
    message= connecter.recieve_message();
    if(message == "ok")
    connecter.send_message("#@#@#");




}

void Main_dialog::get_save_name()
{
    Save_text saver;
    saver.setModal(true);
    saver.exec();



}
void Main_dialog::sign(std::string file_name)
{
    builder.clear_text();
    if(file_name!="")
    {

        std::string message = "sign|save!" + file_name + '~';
        connecter.send_message(message);
    }
    else
    {
        connecter.send_message("sign|dont_save!");
    }


    send_file();

    std::string answer=connecter.recieve_message();
    if(answer=="done")
    {
        QMessageBox::information(this,"Success", "Fisierul a fost trimis cu succes!");

         builder.show();
    }
    else
    {
     QMessageBox::warning(this,"Fail", "Probleme la trimiterea fisierului");
    }

    recieve_file();
    builder.set_txt();
    Text& text_obj_creator=Text::getInstance();
    text_obj_creator.clear_text();
    text_obj_creator.delete_instance();

}
void Main_dialog::on_Build_clicked()
{
    std::string choice=get_choice();
    if(choice == "Sign")
    {
        if(ui->Save->isChecked())
        {
            get_save_name();
        }
        else
        {
            sign("");
        }
    }
    else if (choice== "Verify")
    {
        verify();

    }


}
void Main_dialog::verify()
{
    connecter.send_message("verify|");
    send_file();
    std::string message=connecter.recieve_message();
    if(message!="done")
    {
        QMessageBox::warning(this,"Error", "Probleme la trimiterea fisierului");
    }

    std::string answer=connecter.recieve_message();
    if(answer=="corect")
    {
        QMessageBox::information(this,"Success", "Fisierul trimis este autentic!");

    }
    else if(answer == "incorect")
    {
     QMessageBox::warning(this,"Fail", "Semnatura fisierului nu corespunde utilizatorului!");
    }


    Text& text_obj_creator=Text::getInstance();
    text_obj_creator.clear_text();
    text_obj_creator.delete_instance();
}

void Main_dialog::on_pushButton_signOut_clicked()
{
    this->hide();
    logger = new Loginner(this);
    logger->show();
}

void Main_dialog::on_pushButton_Upload_clicked()
{
    Text& text_obj_creator=Text::getInstance();
    QString file_name = QFileDialog::getOpenFileName(this,"Open the file");
    QFile file(file_name);
    text_obj_creator.set_file_name(file_name);
    text_obj_creator.set_window_title();
    text_obj_creator.show();

}
void Main_dialog::recieve_file()
{
    std::string line;
    std::ofstream writer;
    writer.open("M:/7--------------QT------------------/Loginner/file.txt");
    do
        {
            connecter.send_message("ok");
            line = connecter.recieve_message();
            if (line == "#@#@#")
            {
                connecter.send_message("done");
                writer.close();
            }
            else
            {
                writer<<line;
            }


        } while (line != "#@#@#");
}

void Main_dialog::on_pushButton_Download_clicked()
{

    connecter.send_message("download|files)");
    std::string message = connecter.recieve_message();

    if(message == "access")
    {
        QMessageBox::warning(this,"Error", "Nu aveti acces la acest fisier!");
    }
    else if(message == "ready")
    {

        recieve_file();
    }

    std::ifstream reader;
    std::string files;
    reader.open("M:/7--------------QT------------------/Loginner/file.txt");
    std::string line;
       while (std::getline(reader, line))
       {

           files.append(line);
           files.append("\n");


       }


    downloader.set_files(files);
    downloader.setModal(true);
    downloader.exec();
}

void Main_dialog::on_pushButton_clicked()
{
    Text& text_obj_creator=Text::getInstance();
    text_obj_creator.show();
}
