#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string>
#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <map>

#include "Server_connecter.h"
#include "Base64.h"
#include "KeyManager.h"
#include "Signer.h"
#include "Verifier.h"
#include "Initializer.h"
#include "Saver.h"
#include "Downloader.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "2715"


int main(void)
{
	printf("\t\t\tSERVER\n");


	Initializer& creator = Initializer::getInstance();
	creator.set_path(PATH);
	int aux = creator.check_folders();
	if (aux == 0) 
		creator.create_folders();


	Server_connecter& connecter = Server_connecter::getInstance();
	connecter.initialize_socket();
	connecter.accept_socket();

	code_veryfier& getter = code_veryfier::getInstance();
	enum class Actions : int { Login = 0, Register = 1, Sign = 2, Verify = 3, Save = 4, Download = 5};

	do {

		std::string message = connecter.recieve_message();
		std::string text;
		//std::cout << "am primit:" << message << std::endl;
		if (connecter.get_iResult() >= 0) {
			if (message == "exit")
				break;


			switch (getter.get_action(getter.get_command(message))){

			case (int)Actions::Login:
				KeyManager* key_mng; key_mng = key_mng->GetInstance();
				std::cout << "\ncredentiale username: " << getter.get_username() << std::endl << "parola: " << getter.get_password() << std::endl;
				connecter.send_message(getter.get_login_answer());
				if (getter.get_login_answer()=="corect")
				{
					key_mng->assignKeys(getter.get_command(message));
				}
				
				break;

			case (int)Actions::Register:
				if (getter.set_register_answer() == 1)
					connecter.send_message("folosit");
				else
				{
					if (getter.get_reg_answ() == "corect")
					{
						getter.register_client();

						KeyManager* key_mng; key_mng = key_mng->GetInstance();
						key_mng->assignKeys(getter.get_command(message));

						std::cout << "\ncredentiale username:" << getter.get_username() << "\tparola:" << getter.get_password() << std::endl;
					}
					connecter.send_message(getter.get_reg_answ());
				}
				break;

			case (int)Actions::Sign: 
			{
				Signer signer;
				signer.build();

				break;
			}
			case (int)Actions::Verify:
			{	
				Verifier verifier;
				verifier.build();

				break;
			}
			case (int)Actions::Save:
			{
				Saver saver(getter.get_save_name());
				saver.check_used();
				break;
			}
			case (int)Actions::Download:
			{
				Downloader downloader(getter.get_download_argument());
				downloader.check_used();
				break;
			}
			default:
				break;
			}
		}

		else {
			ExceptionsHandler e("recv failed with error: " + WSAGetLastError());
			connecter.close_socket();
			WSACleanup();
		}

	} while (connecter.get_iResult() >= 0);

	connecter.close_socket();
}