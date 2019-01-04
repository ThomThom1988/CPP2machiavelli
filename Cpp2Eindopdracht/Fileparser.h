#pragma once
#include "Card.h"
#include <vector>
#include <fstream> 

class Fileparser
{
public:
	Fileparser();
	~Fileparser();
	std::vector<std::unique_ptr<Card>> readCharacterCards();
	std::vector<std::unique_ptr<Card>> readBuildingCards();
};

