#include "code_veryfier.h"


code_veryfier* code_veryfier::instance = nullptr;
code_veryfier& code_veryfier::getInstance()
{
	if (code_veryfier::instance == nullptr)
	{
		instance = new code_veryfier();
	}
	return *instance;
}
std::string code_veryfier::get_command(std::string message)
{
	{
		std::string cuvant = "";
		for (auto x : message)
		{
			if (x == '|')
			{
				this->comanda = cuvant;
				cuvant = "";
				
			}
			else if (x == '#'&& this->comanda != "sign")
			{
				this->username = cuvant;
				cuvant = "";
			}
			else if (x == '@' && this->comanda != "sign")
			{
				this->password = cuvant;
				cuvant = "";
			}
			else if (x == '^' && this->comanda == "save")
			{
				this->text_save_name = cuvant;
				cuvant = "";
			}
			else if (x == '!' && this->comanda == "sign")
			{
				this->save_on_server = cuvant;
				cuvant = "";
			}
			else if (x == '~' && this->save_on_server == "save")
			{
				this->text_save_name = cuvant;
				cuvant = "";
			}
			else if (x == ')' && this->comanda == "download")
			{
				this->download_argument = cuvant;
				cuvant = "";
			}
			else
			{
				cuvant = cuvant + x;
			}
		}
	}


	return this->comanda;
}

std::string code_veryfier::get_username()
{
	return this->username;
}

std::string code_veryfier::get_password()
{
	return this->password;
}

std::string code_veryfier::get_login_answer()
{
	Initializer& getter = Initializer::getInstance();
	this->path = getter.get_path();

	std::string file_location = this->path + "\\login.txt";
	std::ifstream reader;
	reader.open(file_location);

	std::string line;
	std::string cuvant = "";
	std::string pass;
	std::string id;
	std::string answer;
	while (getline(reader, line))
	{
		for (auto x : line) {
			if (x == '#') {
				id = cuvant;
				cuvant = "";
			}
			else if (x == '@') {
				pass = cuvant;
				cuvant = "";
			}
			else {
				cuvant = cuvant + x;
			}
		}
		if (id == this->username && this->password == pass)
		{
			return "corect";
		}
	}
	if (reader.peek() == std::ifstream::traits_type::eof())
	{
		return  "incorect";
	}


	return "incorect";
}

std::string code_veryfier::get_reg_answ()
{
	return this->reg_answ;
}

int code_veryfier::set_register_answer()
{
	Initializer& getter = Initializer::getInstance();
	this->path = getter.get_path();
	std::string file_location = this->path + "\\login.txt";
	std::ifstream reader;
	reader.open(file_location);

	std::string line;
	std::string cuvant = "";
	std::string pass;
	std::string id;
	std::string answer;
	while (getline(reader, line))
	{
		for (auto x : line) {
			if (x == '#') {
				id = cuvant;
				cuvant = "";
			}
			else if (x == '@') {
				pass = cuvant;
				cuvant = "";
			}
			else {
				cuvant = cuvant + x;
			}
		}
		if (id == this->username) {
			this->reg_answ = "folosit";
			return 1;
		}
		else {
			this->reg_answ = "corect";
		}
	}
	if (reader.peek() == std::ifstream::traits_type::eof())
	{
		this->reg_answ = "corect";
	}
	return 0;
}

void code_veryfier::register_client()
{

	std::string cale_noua = this->path + "\\Clients\\" + this->username;
	if (_mkdir(cale_noua.c_str()) == 0)
		std::cout << "Folderul:" << this->username << " creat cu succes"; 
	else 
		ExceptionsHandler e("problema la crearea fiserului /" + this->username);
	std::ofstream creator;

	cale_noua = cale_noua + "\\fisiere.txt";
	creator.open(cale_noua);
	creator.close();

	cale_noua = this->path + "\\login.txt";
	
	creator.open(cale_noua, std::ios_base::app);
	std::string credentiale = this->username + "#" + this->password + "@";
	creator << credentiale << std::endl;
	creator.close();
}

int code_veryfier::check_symbols(std::string username, std::string password)
{
	for (auto x : username)
	{
		if (x == '|' || x == '@' || x == '#')
		{
			return 1;
		}
	}
	for (auto y : password)
	{
		if (y == '|' || y == '@' || y == '#')
		{
			return 1;
		}
	}
	return 0;
}

void code_veryfier::initialize_map()
{
	action.insert(std::pair<std::string, Actions>("login", Actions::Login));
	action.insert(std::pair<std::string, Actions>("register", Actions::Register));
	action.insert(std::pair<std::string, Actions>("save", Actions::Save));
	action.insert(std::pair<std::string, Actions>("sign", Actions::Sign));
	action.insert(std::pair<std::string, Actions>("verify", Actions::Verify));
	action.insert(std::pair<std::string, Actions>("download", Actions::Download));

}

int code_veryfier::get_action(std::string command)
{
	auto itr = action.find(command);
	if (itr != action.end())
		return (int)itr->second;
	else
		return -1;
}

std::string code_veryfier::get_save_answer()
{
	return this->save_on_server;
}
std::string code_veryfier::get_save_name()
{
	return this->text_save_name;
}
std::string code_veryfier::get_path()
{
	return this->path;
}

std::string code_veryfier::get_download_argument()
{
	return this->download_argument;
}
