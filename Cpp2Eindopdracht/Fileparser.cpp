#include "Fileparser.h"
#include <fstream>
#include "CharacterCard.h"
#include "BuildingCard.h"


Fileparser::Fileparser()
{
}


Fileparser::~Fileparser()
{
}

std::vector<std::unique_ptr<Card>> Fileparser::readCharacterCards()
{
	std::ifstream fileStream;
	std::vector<std::unique_ptr<Card>> v;
	fileStream.open("karakterkaarten.csv");
	while (!fileStream.eof())
	{
		CharacterCard card;
		fileStream >> card;
		v.push_back(std::make_unique<CharacterCard>(card));
	}	
	fileStream.close();
	return v;
}

std::vector<std::unique_ptr<Card>> Fileparser::readBuildingCards()
{
	std::ifstream fileStream;
	std::vector<std::unique_ptr<Card>> v;
	fileStream.open("Bouwkaarten.csv");
	while (!fileStream.eof())
	{
		BuildingCard card;
		fileStream >> card;
		v.push_back(std::make_unique<BuildingCard>(card));
	}
	fileStream.close();
	return v;
}
