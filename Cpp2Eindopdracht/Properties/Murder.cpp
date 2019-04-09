#include "Murder.h"

bool Murder::useProperty()
{
	int choice;
	bool done{ false };
	while (!done) {
		card->get_player()->get_socket() << "\r\nkies het personage die je wilt vermoorden.\r\n";
		std::vector<std::string> exeptions{ "Moordenaar" };
		game->showCharacterChoices(exeptions);
		bool inputgotten{ false };
		while (!inputgotten) {
			
			inputgotten = card->get_player()->get_socket().readline([&choice,this](std::string input) {
				try { choice = std::stoi(input); }
				catch (...) { card->get_player()->get_socket() << "\r\nkies een juiste waarde.\r\n" << machiavelli::prompt; }
			});
		}
		std::string chosenCharacter;
		switch (choice)
		{
		case 1:
			chosenCharacter = "Moordenaar";
			break;
		case 2:
			chosenCharacter = "Dief";
			break;
		case 3:
			chosenCharacter = "Magiër";
			break;
		case 4:
			chosenCharacter = "Koning";
			break;
		case 5:
			chosenCharacter = "Prediker";
			break;
		case 6:
			chosenCharacter = "Koopman";
			break;
		case 7:
			chosenCharacter = "Bouwmeester";
			break;
		case 8:
			chosenCharacter = "Condottiere";
			break;
		default:
			chosenCharacter = "";
			break;
		}

		if (chosenCharacter != "" && chosenCharacter != "Moordenaar") { game->murderCharacter(chosenCharacter); done = true; setCanUse(false); }
		else { card->get_player()->get_socket() << "\r\nDit karakter kan je niet vermoorden.\r\n"; }
	}
	return true;
}
