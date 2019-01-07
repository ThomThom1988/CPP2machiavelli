#pragma once
#include <vector>
#include "CharacterCard.h"
#include "BuildingCard.h"

class Fileparser
{
public:
	Fileparser();
	~Fileparser();
	std::vector<std::unique_ptr<CharacterCard>> readCharacterCards();
	std::vector<std::unique_ptr<BuildingCard>> readBuildingCards();
};

