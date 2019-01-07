#include "Character.h"
#include "../Properties/DrawCoins.h"
#include "../Properties/DrawCards.h"
#include "../Properties/Build.h"

void Character::executeTurn()
{
	int choice;
	bool done{ false };
	while (!done) {
		character->get_player()->printInfo();
		bool nextChoise{ false };
		while (!nextChoise)
		{
			bool inputgotten{ false };
			while (!inputgotten) {
				inputgotten = character->get_player()->get_socket().readline([&choice, &done, this](std::string input) {
					if (input == "klaar")
					{
						done = true;
					}
					else if (input == "tegenstander")
					{
						game->getOtherPlayer(character->get_player())->printBuildings(character->get_player()->get_socket());
						game->getOtherPlayer(character->get_player())->printGold(character->get_player()->get_socket());
					}
					else
					{
						try { choice = std::stoi(input); }
						catch (...) { character->get_player()->get_socket() << "kies een juiste waarde." << machiavelli::prompt; }
					}
				});
			}
			if (!done)
			{
				nextChoise = properties[nextChoise]->useProperty();
				if (!nextChoise) character->get_player()->get_socket() << "kies een juiste waarde." << machiavelli::prompt;
			}
		}
	}
}

void Character::addStandardChoices()
{
	properties.push_back(std::make_unique<DrawCards>(2));
	properties.push_back(std::make_unique<DrawCoins>(2));
	properties.push_back(std::make_unique<Build>());
}

const Socket & operator<<(Socket & os, Character & c)
{
	std::string result;
	for (int i = 0; i < c.properties.size(); i++)
	{
		result += "[";
		result += std::to_string(i);
		result += "] ";
		result += c.properties[i]->get_description();
		result += "\r\n";
	}
	result += "[tegenstander] Bekijk jouw tegenstanders gebouwen en goud.\r\n";
	result += "[klaar] Beëindig beurt.\r\n";
	return os << result << machiavelli::prompt;
}