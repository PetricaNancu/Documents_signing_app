#ifndef TEXT_H
#define TEXT_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <string>

#include "client_connecter.h"
namespace Ui {
class Text;
}

class Text : public QDialog
{
    Q_OBJECT

public:

    static Text& getInstance();
    void delete_instance();

    void set_file_name(QString file_name){this->file_name = file_name;}
    void set_window_title();
    void set_text();
    void clear_text(){this->text.clear();}
    std::string get_text(){
        set_text();
        return this->text.toStdString();}
    std::string get_file_name(){return this->file_name.toStdString();}


private slots:
    void on_pushButton_clicked();

private:
    QString file_name;
    QString text;
    Ui::Text *ui;


    static Text* instance;
    explicit Text(QWidget *parent = nullptr);
    ~Text();

};

#endif // TEXT_H
