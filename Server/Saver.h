#pragma once

#include<string>
#include<iostream>
#include<fstream>

#include "Server_connecter.h"

class Saver
{
private:
	std::string file_name;
	Server_connecter& connecter = Server_connecter::getInstance();

public:
	void save();
	void create_file();
	void check_used();
	Saver(std::string fileName);
};

