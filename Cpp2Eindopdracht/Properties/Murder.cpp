#include "Murder.h"

bool Murder::useProperty()
{
	std::string choice;
	bool done{ false };
	while (!done) {
		card->get_player()->get_socket() << "\r\ntyp de naam van het personage die je wilt vermoorden.\r\n";
		std::vector<std::string> exeptions{ "Moordenaar" };
		game->showCharacterChoices(exeptions);
		bool inputgotten{ false };
		while (!inputgotten) {
			
			inputgotten = card->get_player()->get_socket().readline([&choice](std::string input) {
				choice = input;
			});
		}
		if (game->characterExists(choice) && choice != "Moordenaar") { game->murderCharacter(choice); done = true; setCanUse(false); }
		else { card->get_player()->get_socket() << "\r\nDit karakter kan je niet vermoorden.\r\n"; }
	}
	return true;
}
