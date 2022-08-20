#include "Downloader.h"
#include "code_veryfier.h"

void Downloader::send_file(std::string fileName)
{
	Server_connecter& connecter = Server_connecter::getInstance();
	code_veryfier& getter = code_veryfier::getInstance();
	std::string file_path = getter.get_path() + "\\Clients\\" + getter.get_username() + "\\" + fileName;


	std::ifstream reader;
	reader.open(file_path);
	std::string message;
	std::string line;
	while (std::getline(reader, line))
	{
		message = connecter.recieve_message();
		line.append("\n");
		if (message == "ok")
			connecter.send_file(line);

	}
	message = connecter.recieve_message();
	if (message == "ok")
		connecter.send_file("#@#@#");

	message = connecter.recieve_message();
	if (message == "done")
	{
		std::cout << "Fisier trimis cu success!" << std::endl;
	}
	
}

void Downloader::check_used()
{
	if (this->file_name == "private" || this->file_name == "public" )
	{
		connecter.send_message("access");
	}
	else if (this->file_name == "files" )
	{
		connecter.send_message("ready");
		code_veryfier& getter = code_veryfier::getInstance();

		send_file("fisiere.txt");
	}
	else
	{
		connecter.send_file("ready");
		send_file(this->file_name);
	}
}

Downloader::Downloader(std::string fileName)
{
	this->file_name = fileName;
}
