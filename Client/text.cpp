#include "text.h"
#include "ui_text.h"

Text::Text(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Text)
{
    ui->setupUi(this);
}

Text::~Text()
{
    delete ui;
}

Text* Text::instance = nullptr;
Text& Text::getInstance()
{
    if (Text::instance==nullptr)
    {
        instance = new Text();
    }
    return *instance;
}
void Text::delete_instance()
{
    if (NULL != instance)
            {
                delete instance;
                instance = NULL;
            }
}
void Text::set_text(){

    this->text = ui->Text_editor->toPlainText();
}

void Text::set_window_title()
{

    QFile file(file_name);
    setWindowTitle("Text file");
    QTextStream in(&file);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
    QMessageBox::warning(this,"Warning","Cannot open file: " + file.errorString());
    }
    this->text = in.readAll();
    ui->Text_editor->setText(text);

    file.close();

}

void Text::on_pushButton_clicked()
{
    set_text();
    this->hide();

}
