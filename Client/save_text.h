#ifndef SAVE_TEXT_H
#define SAVE_TEXT_H

#include <QDialog>
#include <string>
#include <QMessageBox>

#include "client_connecter.h"
namespace Ui {
class Save_text;
}

class Save_text : public QDialog
{
    Q_OBJECT

public:
    explicit Save_text(QWidget *parent = nullptr);
    std::string get_name();
    ~Save_text();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Save_text *ui;

    std::string name;
    Client_connecter& connecter = Client_connecter::getInstance();


};

#endif // SAVE_TEXT_H
