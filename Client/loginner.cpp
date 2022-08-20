#include "loginner.h"
#include "ui_loginner.h"
#include <QMessageBox>
#include <QPixmap>

Loginner::Loginner(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Loginner)
{
    ui->setupUi(this);
    QPixmap pix("M:/7--------------QT------------------/Loginner/poze/test_5.jpg");
    ui->label_picture->setPixmap(pix);
    QPixmap pix_logo("M:/7--------------QT------------------/Loginner/poze/emblem#1.png");
    ui->Logo->setPixmap(pix_logo);
}

Loginner::~Loginner()
{
    delete ui;
}


void Loginner::on_pushButton_Login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_2_password->text();
    QString combine = "login|"+username+"#"+password+"@";
    std::string message = combine.toStdString();
    connecter.send_message(message);


    std::string response;
    response = connecter.recieve_message();
    if(response == "corect")
    {
          hide();
          QMessageBox::information (this, " ","Logare cu succes!");
          main_D_creator.show();
    }
    else if(response == "incorect")
    {
        QMessageBox::warning(this," ", "Username sau parola incorecte");
    }


}

void Loginner::on_pushButton_2_Register_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_2_password->text();
    if (check_symbols(username.toStdString(), password.toStdString()) ==1)
    {
            QMessageBox::warning(this,"WARNING", "simboluri ilegale folosite EX: | @ #");

    }
    else
    {
        QString combine = "register|"+username+"#"+password+"@";
        std::string message = combine.toStdString();
        connecter.send_message(message);


        std::string response;
        response = connecter.recieve_message();
        if(response == "corect")
        {
           hide();
           QMessageBox::information (this, "Inregistrat","Contul a fost logat.");
           main_D_creator.show();
        }
          else if(response == "folosit")
        {
         QMessageBox::warning(this,"WARNING", "Username deja existent");
         }
    }
}
int Loginner::check_symbols(std::string username, std::string password)
{
    for (auto x : username)
    {
        if (x == '|' || x == '@' || x == '#')
        {
            return 1;
        }
    }
    for (auto y : password)
    {
        if (y == '|' || y == '@' || y == '#')
        {
            return 1;
        }
    }
    return 0;
}
