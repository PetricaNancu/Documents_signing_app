#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QDialog>
#include <string>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <string>
#include <QMessageBox>
#include <fstream>

#include "builders_text.h"
#include "client_connecter.h"

namespace Ui {
class Downloader;
}

class Downloader : public QDialog
{
    Q_OBJECT

public:

    static Downloader& getInstance();
    void set_files(std::string files);
    void recieve_file();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Downloader *ui;
     static Downloader* instance;
    Builders_text& builder = Builders_text::getInstance();
    Client_connecter& connecter = Client_connecter::getInstance();

    explicit Downloader(QWidget *parent = nullptr);
    ~Downloader();
};

#endif // DOWNLOADER_H
