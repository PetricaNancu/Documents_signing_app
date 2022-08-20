#include "Initializer.h"

Initializer* Initializer::instance = nullptr;
Initializer& Initializer::getInstance()
{
	if (Initializer::instance == nullptr)
	{
		instance = new Initializer();
	}
	return *instance;
}

bool Initializer::check_folders()
{

	this->path = this->path + "\\Use";
	struct stat st;
	stat(this->path.c_str(), &st);
	int ret = stat(this->path.c_str(), &st);
	return (ret == 0) && (st.st_mode & S_IFDIR);

}

void Initializer::create_folders()
{

	if (_mkdir(this->path.c_str()) == 0)
		std::cout << "Folderul /Use creat cu succes" << std::endl; 
	else 
		ExceptionsHandler e("problema la crearea fiserului /Use");

	std::string aux = this->path + "\\Clients";
	if (_mkdir(aux.c_str()) == 0)
		std::cout << "Folderul /Clients creat cu succes" << std::endl;
	else 
		ExceptionsHandler e("problema la crearea fiserului /Clients");

	std::ofstream creator;
	std::string cale_noua = this->path + "\\login.txt";
	creator.open(cale_noua);
	creator.close();

	cale_noua = this->path + "\\file.txt";
	creator.open(cale_noua);
	creator.close();
}

