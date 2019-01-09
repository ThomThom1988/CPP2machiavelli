#include "Character.h"
#include "../Properties/DrawCoins.h"
#include "../Properties/DrawCards.h"
#include "../Properties/Build.h"
#include <algorithm>
#include "../Properties/CoinForCard.h"

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

void Character::addPurpleChoices()
{
	std::vector<std::unique_ptr<BuildingCard>> buildings {std::move(game->getCurrentPlayer()->get_buildings())};
	//laboratorium
	std::vector<std::unique_ptr<BuildingCard>>::iterator lab =
		std::find_if(buildings.begin(), buildings.end(),
			[&](std::unique_ptr<BuildingCard> & obj) { return obj->get_name() == "Laboratorium"; }
	);
	if (lab != buildings.end())
	{
		properties.push_back(std::make_unique<CoinForCard>(*game));
	}

	//werkplaats
	std::vector<std::unique_ptr<BuildingCard>>::iterator work =
		std::find_if(buildings.begin(), buildings.end(),
			[&](std::unique_ptr<BuildingCard> & obj) { return obj->get_name() == "Werkplaats"; }
	);
	if (work != buildings.end())
	{
		if(game->getCurrentPlayer()->get_player().get_gold() >= 3)
		{
			properties.push_back(std::make_unique<DrawCards>(*character,*game, 2, 0,3));
		}
	}
	//observatorium
	std::vector<std::unique_ptr<BuildingCard>>::iterator observe =
		std::find_if(buildings.begin(), buildings.end(),
			[&](std::unique_ptr<BuildingCard> & obj) { return obj->get_name() == "Observatorium"; }
	);
	if (observe != buildings.end())
	{
		disableChoice("Pak 2 kaart(en) en leg er 1 af.");
		properties.push_back(std::make_unique<DrawCards>(*character, *game, 3, 2));
	}
	//bibliotheek
	std::vector<std::unique_ptr<BuildingCard>>::iterator library =
		std::find_if(buildings.begin(), buildings.end(),
			[&](std::unique_ptr<BuildingCard> & obj) { return obj->get_name() == "Bibliotheek"; }
	);
	if (library != buildings.end())
	{
		disableChoice("Pak 2 kaart(en) en leg er 1 af.");
		properties.push_back(std::make_unique<DrawCards>(*character, *game, 2, 0));
	}
	//school voor magiers
	std::vector<std::unique_ptr<BuildingCard>>::iterator school =
		std::find_if(buildings.begin(), buildings.end(),
			[&](std::unique_ptr<BuildingCard> & obj) { return obj->get_name() == "School voor magiërs"; }
	);
	if (school != buildings.end())
	{
		std::string color;
		game->getCurrentPlayer()->get_socket() << "Kies een kleur die jouw school aanneemt";
		bool colorchoice{ false };
		while (!colorchoice)
		{
			bool inputgotten{false};
			while (!inputgotten) {
				inputgotten = game->getCurrentPlayer()->get_socket().readline([&color](std::string input) {
					color = input;
				});
			}
			if (color == "groen" || color == "geel" || color == "blauw" || color == "rood" || color == "lila") colorchoice = true;
			else game->getCurrentPlayer()->get_socket() << "Kies uit groen/geel/blauw/rood/lila";
		}
		school->get()->set_color(color);
	}
	game->getCurrentPlayer()->set_buildings(std::move(buildings));
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
	result += "[klaar] Beëindig beurt.\r\n";
	return result + machiavelli::prompt;
}