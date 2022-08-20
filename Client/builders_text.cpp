#include "builders_text.h"
#include "ui_builders_text.h"

Builders_text::Builders_text(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Builders_text)
{
    ui->setupUi(this);
}

Builders_text::~Builders_text()
{
    delete ui;
}

Builders_text* Builders_text::instance = nullptr;
Builders_text& Builders_text::getInstance()
{
    if (Builders_text::instance==nullptr)
    {
        instance = new Builders_text();
    }
    return *instance;
}
void Builders_text::on_Save_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Warning","Cannot save file: "+ file.errorString());
        return;
    }
    QTextStream out (&file);
    QString text_new = ui->Text_editor->toPlainText();
    out<<text_new;
    file.close();
    ui->Text_editor->clear();
    hide();

}
void Builders_text::set_txt()
{
    QFile file("M:/7--------------QT------------------/Loginner/file.txt");
    QTextStream in(&file);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
    QMessageBox::warning(this,"Warning","Cannot open file: " + file.errorString());
    }
    this->text=in.readAll();

    ui->Text_editor->setText(text);
}
void Builders_text::clear_text()
{
    this->text="";
    ui->Text_editor->setText("");
}

void Builders_text::on_Close_clicked()
{
    hide();
}
