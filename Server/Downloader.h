#pragma once

#include<string>
#include<iostream>
#include<fstream>

#include "Server_connecter.h"


class Downloader
{

private:
	Server_connecter& connecter = Server_connecter::getInstance();
	std::string file_name;
public:
	
	void send_file(std::string fileName);
	void check_used();
	Downloader(std::string fileName);
	
};

