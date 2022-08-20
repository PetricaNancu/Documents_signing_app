#pragma once

#include <direct.h>
#include <string>
#include <iostream>
#include <fstream>

#include "ExceptionsHandler.h"


class Initializer
{
private:
	std::string path;
	static Initializer* instance;
	Initializer() {}

public:
	static Initializer& getInstance();
	void set_path(std::string Path) { this->path = Path; }
	std::string get_path() { return this->path; }
	bool check_folders();
	void create_folders();

	~Initializer() {};
};

