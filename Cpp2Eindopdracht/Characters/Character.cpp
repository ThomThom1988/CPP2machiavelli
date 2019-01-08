#include "Character.h"
#include "../Properties/DrawCoins.h"
#include "../Properties/DrawCards.h"
#include "../Properties/Build.h"
#include <algorithm>

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
							if (properties.at(choice)->canUse())
							{
								properties.at(choice)->useProperty();
								if (!properties.at(choice)->getConnectedDescription().empty()) disableChoice(properties.at(choice)->getConnectedDescription());
							}
							else character->get_player()->get_socket() << "kies een juiste waarde.\r\n";
							
						}
						catch (...)
						{
							character->get_player()->get_socket() << "kies een juiste waarde.\r\n";
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

void Character::disableChoice(const std::string description)
{
	std::vector<std::unique_ptr<CardProperty>>::iterator object =
		std::find_if(properties.begin(), properties.end(),
			[&](std::unique_ptr<CardProperty> & obj) { return obj->get_description() == description; }
	);
	if (object != properties.end()) {
		object->get()->setCanUse(false);
	}
}


std::string Character::showChoices()
{
	std::string result;
	result += "opties:\r\n";
	int i{0};
	for (auto &property : properties)
	{
		if (property->canUse())
		{
			result += "[";
			result += std::to_string(i);
			result += "] ";
			result += property->get_description();
			result += "\r\n";
		}
		i++;
	}
	result += "[tegenstander] Bekijk jouw tegenstanders gebouwen en goud.\r\n";
	result += "[klaar] Be�indig beurt.\r\n";
	return result + machiavelli::prompt;
}