#pragma once
#include <openssl/err.h>
#include <string>
#include <iostream>


class ExceptionsHandler
{
public:
	ExceptionsHandler(std::string exception) { std::cout << exception << std::endl; };
	~ExceptionsHandler() {};
};

