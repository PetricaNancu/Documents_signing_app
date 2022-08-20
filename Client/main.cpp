#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string>
#include <iostream>


#include "client_connecter.h"
#include "loginner.h"



#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Loginner creator;
    //printf("\t\t\tCLIENT\n");
    std::string message;

    Client_connecter& connecter = Client_connecter::getInstance();
    connecter.init_connection(argv[1]);
    creator.show();





    return a.exec();
}
