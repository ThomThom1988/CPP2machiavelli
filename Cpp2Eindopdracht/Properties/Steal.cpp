#include "Steal.h"

bool Steal::useProperty()
{
	int choice;
	bool done{ false };
	while (!done) {
		card->get_player()->get_socket() << "\r\nkies het personage die je wilt bestelen.\r\n";
		std::vector<std::string> exceptions{ "Moordenaar","Dief",game->getMurderedCharacter() };
		game->showCharacterChoices(exceptions);
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
		if (chosenCharacter == "Moordenaar" || chosenCharacter == "Dief" || chosenCharacter == game->getMurderedCharacter())
			card->get_player()->get_socket() << "\r\nDit karakter kan je niet bestelen.\r\n";
		else if (chosenCharacter == "")
			card->get_player()->get_socket() << "\r\nDit karakter bestaat niet.\r\n";
		else {
			game->stealFromCharacter(chosenCharacter);
			done = true;
			setCanUse(false);
		}
	}
	return true;
}
