#include "Build.h"

bool Build::useProperty()
{
	int choice;
	bool done{ false };
	while (!done) {
		card->get_player()->get_socket() << "\r\nKies welk gebouw je wilt bouwen.\r\n";
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
						if (times == 0 && !game->inCheatMode()) setCanUse(false);
						if (game->getCurrentPlayer()->getAmountOfBuildings() >= 8 && !game->getOtherPlayer(game->getCurrentPlayer())->firstToGet8)
						{
							game->gameEndsAfterRound();
							game->getCurrentPlayer()->firstToGet8 = true;
						}
					}
					else{ card->get_player()->get_socket() << "\r\nKies een gebouw in je hand die je kunt betalen.\r\n"; }
				}
			});
		}
	}
	return true;
}
