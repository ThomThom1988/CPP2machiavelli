#pragma once
#include "CharacterCard.h"

class Turn
{
public:
	Turn(const CharacterCard& characterCard);
	~Turn();
	void chooseOption(const int option);
	friend std::ostream & operator<<(std::ostream & os, Turn & t);

private:
	CharacterCard character;
	std::vector<std::unique_ptr<CardProperty>> properties;	
	bool standardOptionChosen;
};

