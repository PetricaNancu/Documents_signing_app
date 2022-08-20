#include "downloader.h"
#include "ui_downloader.h"

Downloader::Downloader(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Downloader)
{
    ui->setupUi(this);
}
Downloader* Downloader::instance = nullptr;
Downloader& Downloader::getInstance()
{
    if (Downloader::instance==nullptr)
    {
        instance = new Downloader();
    }
    return *instance;
}
Downloader::~Downloader()
{
    delete ui;
}
void  Downloader::set_files(std::string files)
{
    ui->lineEdit->clear();
    QString aux = QString::fromStdString(files);
    if(files == "")
    {
        ui->label_files->setText("\tNu aveti fisiere");
        builder.clear_text();
    }
    else
    {
         ui->label_files->setText(aux);
    }


}
void Downloader::recieve_file()
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

void Downloader::on_pushButton_clicked()
{
    hide();
    QString aux = ui->label_files->text();


    QString file_name = ui->lineEdit->text();
    std::string auxiliar = file_name.toStdString();

    std::string message = "download|" + auxiliar + ')';
    connecter.send_message(message);
    message = connecter.recieve_message();

    if(message == "access")
    {
        QMessageBox::warning(this,"Error", "Nu aveti acces la acest fisier!");
    }
    else if(message == "ready")
    {

        recieve_file();
    }


   if(aux != "\tNu aveti fisiere" && auxiliar != "")
   {
       builder.clear_text();
       builder.set_txt();
       builder.setModal(true);
       builder.exec();
   }

}
