#include "Saver.h"

#include "code_veryfier.h"
Saver::Saver(std::string fileName)
{
	this->file_name = fileName;
}

void Saver::save()
{
	
	code_veryfier& getter = code_veryfier::getInstance();
	std::string file_path = getter.get_path() + "\\Clients\\" + getter.get_username() + "\\fisiere.txt";

	std::ofstream creator;
	creator.open(file_path, std::ios_base::app);
	creator << this->file_name << "\n";

	create_file();
}

void Saver::create_file()
{
	code_veryfier& getter = code_veryfier::getInstance();
	std::string file_path = getter.get_path() + "\\file.txt";

	std::ifstream reader;
	reader.open(file_path);
	std::string line;
	std::string fisier;
	while (std::getline(reader, line))
	{
		fisier.append(line);
		fisier.append("\n");
	}
	reader.close();

	std::ofstream writer;
	file_path = getter.get_path() + "\\Clients\\" + getter.get_username() + "\\" + getter.get_save_name();
	writer.open(file_path);
	writer << fisier;
	writer.close();
}

void Saver::check_used()
{
	code_veryfier& getter = code_veryfier::getInstance();
	std::string file_path = getter.get_path() + "\\Clients\\" + getter.get_username() + "\\fisiere.txt";

	std::ifstream reader;
	reader.open(file_path);
	std::string line;
	std::string folosit;
	while (std::getline(reader, line))
	{
		if (line == this->file_name)
		{
			folosit = "folosit";
			break;
		}
		
	}
	reader.close();
	
	
	if (folosit == "folosit")
	{
		connecter.send_message(folosit);
	}
	else
	{
		connecter.send_message("nefolosit");
	}
	
}


