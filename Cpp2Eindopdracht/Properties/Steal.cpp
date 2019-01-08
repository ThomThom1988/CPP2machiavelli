#include "Steal.h"

bool Steal::useProperty()
{
	std::string choice;
	bool done{ false };
	while (!done) {
		card->get_player()->get_socket() << "typ de naam van het personage die je wilt bestelen.\r\n";
		std::vector<std::string> exceptions{ "Moordenaar","Dief",game->getMurderedCharacter() };
		game->showCharacterChoices(exceptions);
		bool inputgotten{ false };
		while (!inputgotten) {

			inputgotten = card->get_player()->get_socket().readline([&choice](std::string input) {
				choice = input;
			});
		}
		if (choice == "Moordenaar" || choice == "Dief" || choice == game->getMurderedCharacter())
			card->get_player()->get_socket() << "Dit karakter kan je niet bestelen.\r\n";
		else if (!game->characterExists(choice))
			card->get_player()->get_socket() << "Dit karakter bestaat niet.\r\n";
		else game->stealFromCharacter(choice); done = true;
	}
	return true;
}
