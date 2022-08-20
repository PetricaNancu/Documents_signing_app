#include "save_text.h"
#include "ui_save_text.h"

#include "main_dialog.h"

Save_text::Save_text(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Save_text)
{
    ui->setupUi(this);
}

Save_text::~Save_text()
{
    delete ui;
}


void Save_text::on_pushButton_clicked()
{
    QString aux = ui->lineEdit->text();
    this->name = aux.toStdString();

    std::string message = "save|"+ this->name + '^';
    connecter.send_message(message);

    std::string name_used=connecter.recieve_message();
    if(name_used == "folosit")
    {
         QMessageBox::warning(this,"Error", "Numele fisierului este utilizat");
    }
    else if (name_used == "nefolosit")
    {
        Main_dialog &builder = Main_dialog::get_instance();
        builder.sign(this->name);
        hide();
    }

}
std::string Save_text::get_name()
{
    return this->name;
}
