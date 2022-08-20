#define _CRT_SECURE_NO_WARNINGS

#include<string>
#include<iostream>
#include<fstream>
#include <map>

#include "Initializer.h"
#include "Server_connecter.h"

class code_veryfier
{

private:
	static code_veryfier* instance;
	
	std::string comanda;
	std::string username;
	std::string password;
	std::string path;
	std::string reg_answ;
	std::string save_on_server;
	std::string text_save_name;
	std::string download_argument;

	char cpy[100];
	char *p;
	code_veryfier() { initialize_map(); }

	enum class Actions : int { Login = 0, Register = 1, Sign = 2, Verify = 3, Save = 4 , Download = 5};
	std::map<std::string, Actions> action;

public:
	static code_veryfier& getInstance();
	std::string get_command(std::string message);
	std::string get_username();
	std::string get_password();
	std::string get_login_answer();
	std::string get_reg_answ();
	std::string get_save_answer();
	std::string get_save_name();
	std::string get_path();
	std::string get_download_argument();

	int set_register_answer();
	void register_client();
	int check_symbols(std::string username, std::string password);

	void initialize_map();
	int get_action(std::string command);
};