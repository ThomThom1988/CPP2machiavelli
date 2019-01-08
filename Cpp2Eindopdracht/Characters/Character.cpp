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
		character->get_player()->get_socket() << "Je bent nu de " << character->get_name() << ".\r\n" << showChoices();
		bool nextChoise{ false };
		while (!nextChoise)
		{
				nextChoise = character->get_player()->get_socket().readline([&choice, &done, this](std::string input) {
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
						try
						{
							choice = std::stoi(input);
							properties.at(choice)->useProperty();
						}
						catch (...)
						{
							character->get_player()->get_socket() << "kies een juiste waarde." << machiavelli::prompt;
						}
						
					}
				});			
		}
	}
}

void Character::addStandardChoices()
{
	properties.push_back(std::make_unique<DrawCards>(*character, *game,2,1));
	properties.push_back(std::make_unique<DrawCoins>(*character, *game, 2));
	properties.push_back(std::make_unique<Build>(*character, *game,1));
}


std::string Character::showChoices()
{
	std::string result;
	result += "opties:\r\n";
	for (int i = 0; i < properties.size(); i++)
	{
		result += "[";
		result += std::to_string(i);
		result += "] ";
		result += properties[i]->get_description();
		result += "\r\n";
	}
	result += "[tegenstander] Bekijk jouw tegenstanders gebouwen en goud.\r\n";
	result += "[klaar] Beëindig beurt.\r\n";
	return result + machiavelli::prompt;
}