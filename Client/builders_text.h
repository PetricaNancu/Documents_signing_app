#ifndef BUILDERS_TEXT_H
#define BUILDERS_TEXT_H

#include <QDialog>
#include <string>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <string>
#include <QMessageBox>


namespace Ui {
class Builders_text;
}

class Builders_text : public QDialog
{
    Q_OBJECT

public:
     static Builders_text& getInstance();
     void set_txt();
     void clear_text();

private slots:
    void on_Save_clicked();

    void on_Close_clicked();

private:
    Ui::Builders_text *ui;

    QString text;


    static Builders_text* instance;
    explicit Builders_text(QWidget *parent = nullptr);
    ~Builders_text();

};

#endif // BUILDERS_TEXT_H
