#include "Build.h"

bool Build::useProperty()
{
	int choice;
	bool done{ false };
	while (!done) {
		card->get_player()->get_socket() << "Kies welk gebouw je wilt bouwen.\r\n";
		card->get_player()->printHand();
		card->get_player()->printGold(card->get_player()->get_socket());
		game->getCurrentPlayer()->get_socket() << "[annuleren] ga terug naar keuze.\r\n";
		bool nextChoise{ false };
		while (!nextChoise)
		{
			nextChoise = card->get_player()->get_socket().readline([&choice, &done, this](std::string input) {
				if (input == "annuleren")
				{
					done = true;
				}
				else
				{
					choice = std::stoi(input);
					auto canBuild = card->get_player()->addBuilding(choice);
					if(canBuild)
					{
						done = true;
						times--;
					}
					else{ card->get_player()->get_socket() << "Kies een gebouw in je hand die je kunt betalen.\r\n"; }
				}
			});
		}
	}
	return true;
}
